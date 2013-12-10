<?php
	class mwp_pay {

		private $api;
		
		public $wallet_id;
		public $card_registration;
		public $card;
		public $user_id;

		public $payIn;

		public function __construct() {
			//Get SDK interface
			require_once __DIR__ . "/mwp_api.inc";
			$this -> api = mwp_get_api();

			require_once __DIR__ . "/mwp_user.inc";
			$this -> po_default_values ();
		}

		private function po_manage_api_error ( $e, $details, $source ) {
			if ( $details ) {
				foreach ($e->GetErrorDetails() -> Errors as $error) {
					$output .= $error . " ";
				}
	
				//See: mangopay_wp_plugin_errors.php
				$_SESSION["MWP_API_ERROR"] .= "[{$source}] " . __( "mangopay_error", 'mangopay_wp_plugin') . " [{$output}]"; 
			} else {
				$_SESSION["MWP_API_ERROR"] .= "[{$source}]" . $e->GetMessage();
			}
			return;
		}

		private function po_get_fields ( $type ) {
			require_once ( __DIR__ . '/includes/mwp_fields.inc');
			return mwp_get_fields ( $yFields );
		}

		public function mwp_new_user () {
			$user = new mwp_user;
			$this -> user_id = $user -> mwp_save();
			return $this -> user_id;
		}

		private function po_default_values () {

			//User section
			if ( $user = wp_get_current_user() ) {
				$this -> Email = $user -> user_email;
				$this-> FirstName = $user -> user_firstname;
				$this -> LastName = $user -> user_lastname;
			}

			$this -> Nationality = 'ES';
			$this -> CountryOfResidence = 'ES';

			$this -> LegalRepresentativeNationality = 'ES';
			$this -> LegalRepresentativeCountryOfResidence = 'ES';

			$this -> LegalRepresentativeNationality = 'ES';
			$this -> LegalRepresentativeCountryOfResidence = 'ES';
			
			//Just while coding ¡¡remove!!
			$this -> Birthday = "1950-01-01";
			$this -> cardNumber = "4970101122334406";
			$this -> cardDate = "1220";
			$this -> cardCvx = "123";
			//Pay section
			$this -> Amount = $_POST['amount'];
			$this -> Post = get_the_title( $_POST['post_id'] );
		}

		public function mwp_preregister_card () {

			$cardNumber = $_POST['mwp_cardNumber'];
			$cardDate = $_POST['mwp_cardDate']; 
			$cardCvx = $_POST['mwp_cardCvx'];

			if ( !isset($cardNumber) || !isset($cardDate ) || !isset($cardCvx) ) 
				return null;

			try {
				$cardRegister = new \MangoPay\CardRegistration();
				$cardRegister->UserId = $this -> user_id;
				$cardRegister->Currency = 'EUR';
				$cardRegister->CardType = 'VISA';
				$createdCardRegister = $this->api->CardRegistrations->Create($cardRegister);

			} catch (MangoPay\ResponseException $e) {
				$this -> po_manage_api_error ($e, true, 'mwp_preregister_card');
				return;

			} catch (MangoPay\Exception $e) {
				$this -> po_manage_api_error ($e, false, 'mwp_preregister_card');
				return;
			}

			$this -> card_registration = $createdCardRegister;

			//Build params 
			$data = array(
				'data' => $createdCardRegister->PreregistrationData,
				'accessKeyRef' => $createdCardRegister->AccessKey,
				'cardNumber' => $cardNumber,
				'cardExpirationDate'=> $cardDate,
				'cardCvx' => $cardCvx,
				'return_url' => plugin_dir_url (__FILE__) . '/mwp_payment.php'
			);
			return $data;
		}

		public function mwp_validate_card ( $data ) {
			
			try {
				$cardRegister = $this->api->CardRegistrations->Get($this->card_registration->Id);
				$cardRegister->RegistrationData = $data;
				$updatedCardRegister = $this->api->CardRegistrations->Update($cardRegister);

				if ($updatedCardRegister->Status != 'VALIDATED' || !isset($updatedCardRegister->CardId)) {
					$_SESSION["MWP_API_ERROR"] = 'Cannot create virtual card. Payment has not been created.';
					return;
				}

				$this->card = $this->api->Cards->Get($updatedCardRegister->CardId);

				return true;

			} catch (MangoPay\ResponseException $e) {
				$this -> po_manage_api_error ($e, true, 'mwp_validate_card');

			} catch (MangoPay\Exception $e) {
				$this -> po_manage_api_error ($e, false, 'mwp_validate_card');
			}
		}

		public function mwp_wallet_for_post ( $post_id ) {

			$post = get_post ( $post_id );
			$user = get_userdata ( $post->post_author );
			try {
				if ( $post -> wallet_id )
					$wallet = $this->api->Wallets->Get( $post -> wallet_id );
				else {
					$wallet = new \MangoPay\Wallet();
					$wallet->Owners = array( $user -> mangopay_id );
					$wallet->Currency = 'EUR';
					$wallet->Description = 'Temporary wallet for payment demo';
					$wallet = $this->api->Wallets->Create($wallet);
					update_post_meta( $post_id, 'wallet_id', $wallet -> Id );
				}
			} catch (MangoPay\ResponseException $e) {
				$this -> po_manage_api_error ($e, true, 'mwp_wallet_for_post');

			} catch (MangoPay\Exception $e) {
				$this -> po_manage_api_error ($e, false, 'mwp_wallet_for_post');
			}

			if ( $wallet ) 
				$this -> wallet_id = $wallet->Id;
		}

		public function mwp_payIn ( $amount, $fees ) {
				//Parse amount to cents
				$amount = $amount * 100;

				// create pay-in CARD DIRECT
				$payIn = new \MangoPay\PayIn();
				$payIn->CreditedWalletId = $this->wallet_id;
				$payIn->AuthorId = $this->user_id;
				$payIn->DebitedFunds = new \MangoPay\Money();
				$payIn->DebitedFunds->Amount = $amount;
				$payIn->DebitedFunds->Currency = 'EUR';
				$payIn->Fees = new \MangoPay\Money();
				$payIn->Fees->Amount = $fees;
				$payIn->Fees->Currency = 'EUR';
			try {
				// payment type as CARD
				$payIn->PaymentDetails = new \MangoPay\PayInPaymentDetailsCard();
				if ($this->card->CardType == 'CB' || $this->card->CardType == 'VISA' || $this->card->CardType == 'MASTERCARD')
					$payIn->PaymentDetails->CardType = 'CB_VISA_MASTERCARD';
				elseif ($card->CardType == 'AMEX')
					$payIn->PaymentDetails->CardType = 'AMEX';

				// execution type as DIRECT
				$payIn->ExecutionDetails = new \MangoPay\PayInExecutionDetailsDirect();
				$payIn->ExecutionDetails->CardId = $this->card->Id;
				$payIn->ExecutionDetails->SecureModeReturnURL = get_bloginfo('url');

				// create Pay-In
				$createdPayIn = $this->api->PayIns->Create($payIn);

				if ($createdPayIn->Status == 'SUCCEEDED') {
					return '<div style="color:green;">'.
						    'Pay-In has been created successfully. '
						    .'Pay-In Id = ' . $createdPayIn->Id 
						    . ', Wallet Id = ' . $this -> wallet_id  
						. '</div>';
				}  else {
				// if created Pay-in object has status different than SUCCEEDED 
				// that occurred error and display error message
					return '<div style="color:red;">'.
						    'Pay-In has been created with status: ' 
						    . $createdPayIn->Status . ' (result code: '
						    . $createdPayIn->ResultCode . ')'
						.'</div>';
				}

			} catch (MangoPay\ResponseException $e) {
				$this -> po_manage_api_error ($e, true, 'mwp_payIn');

			} catch (MangoPay\Exception $e) {
				$this -> po_manage_api_error ($e, false, 'mwp_payIn');
			}
			
			if ( $payIn )
				$this -> payIn = $createdPayIn;
		}
	
	}
?>
