<?php

namespace mwp;

use MangoPay as m;

	class mwp_bank extends m\BankAccount{

		public function __construct ( $wp_user = null, $autosave = false) {

			if ( $wp_user )
				$this -> wp_id = $wp_user->ID;

			if ( $wp_user -> mangopay_id )
				$this -> mangopay_id = $wp_user -> mangopay_id;

			if ( $wp_user -> bank_id )
				$this -> bank_id = $wp_user -> bank_id;

			if ( $autosave ) 
				self::mwp_save_bank ( $this );

			try {
				if ( $this -> mangopay_id && $this -> bank_id )
					$bank = mwp_api::get_instance()->Users->GetBankAccount($wp_user -> mangopay_id, $this -> bank_id);

			} catch (m\ResponseException $e) {
				mwp_errors::mwp_manage_api_error ($e, true, 'mwp_user.mwp_construct');
			} catch (m\Exception $e) {
				mwp_errors::mwp_manage_api_error ($e, false, 'mwp_user.mwp_construct');
			}

			if ( $bank ) 
				self::mwp_set_values ( $bank );
		}

		public function mwp_set_values( $bank ) {

			if ( $bank ) {
				$yFields =  mwp_get_fields ( 'bank' );
				foreach ( $yFields as $field )
					$this -> $field = $bank -> $field;
			}

		}

		static public function mwp_get_banks ( $user ) {

			//Cannot save for non mangopay users
			if ( ! $user -> mangopay_id )
				return mwp_errors::mwp_manage_error ("mwp_bank. mwp_save_bank. No mangopay id found.<br>\n", 'mwp_bank.mwp_save_bank');
			
			$banks = mwp_api::get_instance()-> Users -> GetBankAccounts ( $user -> mangopay_id );
			foreach ( $banks as $bank )
				$output_banks[$bank->BIC] = $bank->IBAN; 

			return $output_banks;
		}

		static public function mwp_save_bank ( $bank ) {

			//Cannot save for non mangopay users
			if ( ! $bank -> mangopay_id )
				return mwp_errors::mwp_manage_error ("mwp_bank. mwp_save_bank. No mangopay id found.<br>\n", 'mwp_bank.mwp_save_bank');

			//Get fields names switching on user type
			$yFields = mwp_get_fields ( 'bank' );
			if ( ! mwp_has_changed_fields( $yFields, $bank ) )
				return;

			//New object
			$bankAccount = new m\BankAccount();
			$bankAccount -> UserId = $bank -> mangopay_id;
			$bankAccount -> Type = 'IBAN';

			//Fill data
			foreach ( $yFields as $field ) 
				$bankAccount -> $field = $_POST["mwp_{$field}"];

			try {
				$bankResult = mwp_api::get_instance()-> Users -> CreateBankAccount ( $bank -> mangopay_id, $bankAccount );
				update_user_meta( $bank->wp_id, 'bank_id', $bankResult -> Id );

				return true;

			} catch (m\ResponseException $e) {
				mwp_errors::mwp_manage_api_error ($e, true, 'mwp_bank.mwp_save_bank');

			} catch (m\Exception $e) {
				mwp_errors::mwp_manage_api_error ($e, false, 'mwp_bank.mwp_save_bank');
			}
		}

	}




?>
