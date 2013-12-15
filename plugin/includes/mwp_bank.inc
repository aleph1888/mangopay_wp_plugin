<?php

namespace mwp;

use MangoPay as m;

	class mwp_bank {

		static public function mwp_get_banks ( $user ) {

			//Cannot save for non mangopay users
			if ( ! $user -> mangopay_id )
				return "mwp_bank. mwp_save_bank. No mangopay id found.<br>\n";
			
			$banks = mwp_api::get_instance()-> Users -> GetBankAccounts ( $user -> mangopay_id );
			foreach ( $banks as $bank )
				$output_banks[$bank->BIC] = $bank->IBAN; 

			return $output_banks;
		}

		static public function mwp_save_bank ( $user ) {

			//Cannot save for non mangopay users
			if ( ! $user -> mangopay_id )
				return "mwp_bank. mwp_save_bank. No mangopay id found.<br>\n";

			//Get fields names switching on user type
			$yFields = mwp_get_fields ( 'bank' );

			//New object
			$bankAccount = new m\BankAccount();
			$bankAccount -> UserId = $user -> mangopay_id;
			$bankAccount -> Type = 'IBAN';

			//Fill data
			foreach ( $yFields as $field ) 
				$bankAccount -> $field = $_POST["mwp_{$field}"];

			try {
				$bankResult = mwp_api::get_instance()-> Users -> CreateBankAccount ( $user -> mangopay_id, $bankAccount );
				$user -> bank_id = $bankResult -> Id; //TODO: manage array.
				return true;

			} catch (m\ResponseException $e) {
				mwp_errors::mwp_manage_api_error ($e, true, 'mwp_bank.mwp_save_bank');

			} catch (m\Exception $e) {
				mwp_errors::mwp_manage_api_error ($e, false, 'mwp_bank.mwp_save_bank');
			}
		}

	}




?>
