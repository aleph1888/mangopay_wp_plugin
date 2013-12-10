<?php 
	class mwp_payout {
		
		private $api;

		// Creates new user in mangopay from post data
		public function __construct ( $user = null ) {

			//Reset errors
			$_SESSION["MWP_API_ERROR"] = null;

			//Get SDK interface
			require_once __DIR__ . "/mwp_api.inc";
			$this -> api = mwp_get_api();

			require_once ( __DIR__ . "/mwp_fields.inc" );

		}

		private function payout_manage_api_error ( $e, $details, $source ) {
			if ( $details ) {
				foreach ($e->GetErrorDetails() -> Errors as $error)
					$output .= $error . " ";

				$_SESSION["MWP_API_ERROR"] .= "[{$source}] {$output}"; 
			} else {
				$_SESSION["MWP_API_ERROR"] .= "[{$source}] {$e->GetMessage()}";
			}
			return;
		}

		//Returns Bankaccount only inf new
		public function mwp_bankaccount_save ( $user = null ) {

			//Cannot save for non mangopay users
			if ( ! $user -> mangopay_id )
				return null;

			//Get fields names switching on user type
			$yFields = mwp_get_fields ( 'bank' );


				//New object
				$bankAccount = new MangoPay\BankAccount();
				$bankAccount -> UserId = $user -> mangopay_id;
				$bankAccount -> Type = 'IBAN';

				//Fill data
				foreach ( $yFields as $field ) 
					$bankAccount -> $field = $_POST["mwp_{$field}"];

			try {
				$bankResult = $this -> api -> Users -> CreateBankAccount ( $user -> mangopay_id, $bankAccount );
				return $bankResult -> Id;

			} catch (MangoPay\ResponseException $e) {
				$this -> payout_manage_api_error ($e, true, 'mwp_payout.mwp_bankaccount_save');

			} catch (MangoPay\Exception $e) {
				$this -> payout_manage_api_error ($e, false, 'mwp_payout.mwp_bankaccount_save');
			}

		}

		//Returns Bankaccount only inf new
		public function mwp_bankaccount_get_info ( $user_id ) {

			$user = get_userdata ( $user_id );
			if ( ! $user -> bank_id )
				return __( "must_fill_bankdata", 'mangopay_wp_plugin');

			try {
				//New object
				$bankAccount = new MangoPay\BankAccount();
				$bankAccountResult = $this -> api -> Users -> GetBankAccount ( $user -> mangopay_id, $user -> bank_id );
				require_once ( __DIR__ . "/mwp_forms.inc" );
				return mwp_forms::mwp_show_bank_section_info ( $bankAccountResult );

			} catch (MangoPay\ResponseException $e) {
				$this -> payout_manage_api_error ($e, true, 'mwp_payout.mwp_bankaccount_save');

			} catch (MangoPay\Exception $e) {
				$this -> payout_manage_api_error ($e, false, 'mwp_payout.mwp_bankaccount_save');
			}

		}

		public function mwp_do_payout ( $user_id, $wallet_id, $amount, $bank_id ) {

				$payout = new MangoPay\PayOut();

				$payout -> DebitedWalletId = $wallet_id;
				$payout -> PaymentType = 'BANK_WIRE';
				$payout -> AuthorId = $user_id;
					$money_amount = new MangoPay\Money();
					$money_amount ->  Amount = $amount; 
					$money_amount ->  Currency = 'EUR';
				$payout -> DebitedFunds = $money_amount;
					$money_fees = new MangoPay\Money();
					$money_fees ->  Amount = 0; 
					$money_fees ->  Currency = 'EUR';
				$payout -> Fees = $money_fees;

				$payoutdetails = new MangoPay\PayOutPaymentDetailsBankWire();
				$payoutdetails -> BankAccountId = $bank_id;
				$payout -> MeanOfPaymentDetails = $payoutdetails;

			try {
				$newPayout = $this -> api -> PayOuts -> Create ($payout);

				return $newPayout -> Status;
			} catch (MangoPay\ResponseException $e) {
				$this -> payout_manage_api_error ($e, true, 'mwp_payout.mwp_do_payout');

			} catch (MangoPay\Exception $e) {
				$this -> payout_manage_api_error ($e, false, 'mwp_payout.mwp_do_payout');
			}
		}
	}

?>
