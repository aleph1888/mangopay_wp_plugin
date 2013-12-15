<?php
namespace mwp;

use MangoPay as m;

	class mwp_pay {

		private $api;

		public $user;
		
		public $wallet_id;
		public $card_registration;
		public $card;
		public $Alias;

		public $Amount;
		public $Post; // Title

		public $payIn;

		//If $autosave_user then, will read post params to create a user
		public function __construct( $user, $autosave_user = false ) {
			$this->api = mwp_api::get_instance();

			$this -> user = new mwp_user ( $user, $autosave_user );

			self::po_default_values ();
		}

		private function po_default_values () {

			$this -> Alias = '--';
			if ( $this->user->card_id ) {
				try {
					$this -> card = $this->api->Cards->Get($this->user->card_id);
					$this -> Alias = $this -> card -> Alias;
				} catch (m\ResponseException $e) {
					mwp_errors::mwp_manage_api_error ($e, true, 'mwp_payIn');

				} catch (m\Exception $e) {
					mwp_errors::mwp_manage_api_error ($e, false, 'mwp_payIn');
				}
			}

			$this -> cardNumber = "4970101122334406";
			$this -> cardDate = "1220";
			$this -> cardCvx = "123";

			$this -> Amount = $_REQUEST['mwp_Amount'];
			$this -> Post = get_the_title( $_REQUEST['mwp_post_id'] );
		}

		public function mwp_preregister_card () {

			$cardNumber = $_POST['mwp_cardNumber'];
			$cardDate = $_POST['mwp_cardDate']; 
			$cardCvx = $_POST['mwp_cardCvx'];

			if ( !isset($cardNumber) || !isset($cardDate ) || !isset($cardCvx) ) 
				return null;

			try {
				$cardRegister = new m\CardRegistration();
				$cardRegister->UserId = $this->user->mangopay_id;
				$cardRegister->Currency = 'EUR';
				$cardRegister->CardType = 'CB_VISA_MASTERCARD';
				$createdCardRegister = $this->api->CardRegistrations->Create($cardRegister);

			} catch (m\ResponseException $e) {
				mwp_errors::mwp_manage_api_error ($e, true, 'mwp_preregister_card');
				return;
			} catch (m\Exception $e) {
				mwp_errors::mwp_manage_api_error ($e, false, 'mwp_preregister_card');
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
				'return_url' => plugin_dir_url (__FILE__) . 'mwp_payment.php'
			);
			return $data;

		}

		public function mwp_validate_card ( $data ) {

			try {
				$cardRegister = $this->api->CardRegistrations->Get($this->card_registration->Id);
				$cardRegister->RegistrationData = $data;
				$updatedCardRegister = $this->api->CardRegistrations->Update($cardRegister);

				if ($updatedCardRegister->Status != 'VALIDATED' || !isset($updatedCardRegister->CardId)) 
					return mwp_errors::mwp_manage_error ('Cannot create virtual card. Payment has not been created. Card status ' . $updatedCardRegister->Status);

				$this->card = $this->api->Cards->Get($updatedCardRegister->CardId);

				$this -> user -> card_id = $this->card->Id;

				//If we are doing payment with know user, save id.
				if ( $this -> user -> wp_id )
					update_user_meta( $this -> user -> wp_id, 'card_id', $this->card->Id );

				return true;

			} catch (m\ResponseException $e) {
				mwp_errors::mwp_manage_api_error ($e, true, 'mwp_validate_card');

			} catch (m\Exception $e) {
				mwp_errors::mwp_manage_api_error ($e, false, 'mwp_validate_card');
			}

		}

		public function mwp_wallet_for_post ( $post_id ) {

			$post = get_post ( $post_id );
			$user = get_userdata ( $post->post_author );
			try {
				if ( $post -> wallet_id )
					$wallet = $this->api->Wallets->Get( $post -> wallet_id );
				else {
					$wallet = new m\Wallet();
					$wallet->Owners = array( $user -> mangopay_id );
					$wallet->Currency = 'EUR';
					$wallet->Description = 'Temporary wallet for payment demo';
					$wallet = $this->api->Wallets->Create($wallet);
					update_post_meta( $post_id, 'wallet_id', $wallet -> Id );
				}
			} catch (m\ResponseException $e) {
				mwp_errors::mwp_manage_api_error ($e, true, 'mwp_wallet_for_post');

			} catch (m\Exception $e) {
				mwp_errors::mwp_manage_api_error ($e, false, 'mwp_wallet_for_post');
			}

			if ( $wallet ) 
				$this -> wallet_id = $wallet->Id;

		}

		public function mwp_payIn ( $amount, $fees ) {

			//Parse amount to cents
			$amount = $amount * 100;

			// create pay-in CARD DIRECT
			$payIn = new m\PayIn();
			$payIn->CreditedWalletId = $this->wallet_id;
			$payIn->AuthorId = $this->user->mangopay_id;
			$payIn->DebitedFunds = new m\Money();
			$payIn->DebitedFunds->Amount = $amount;
			$payIn->DebitedFunds->Currency = 'EUR';
			$payIn->Fees = new m\Money();
			$payIn->Fees->Amount = $fees;
			$payIn->Fees->Currency = 'EUR';

			try {
				// payment type as CARD
				$payIn->PaymentDetails = new m\PayInPaymentDetailsCard();
				$payIn->PaymentDetails->CardType = 'CB_VISA_MASTERCARD';
			
				// execution type as DIRECT
				$payIn->ExecutionDetails = new m\PayInExecutionDetailsDirect();
				$payIn->ExecutionDetails->CardId = $this->user->card_id;
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

			} catch (m\ResponseException $e) {
				mwp_errors::mwp_manage_api_error ($e, true, 'mwp_payIn');

			} catch (m\Exception $e) {
				mwp_errors::mwp_manage_api_error ($e, false, 'mwp_payIn');
			}
			
			if ( $payIn )
				$this -> payIn = $createdPayIn;
		}
	
	}
?>
