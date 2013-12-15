<?php 

namespace mwp;

use MangoPay as m;

	class mwp_user {

		public $wp_id; 		//Wordpress user id
		public $mangopay_id; 	//Mangopay user id
		public $card_id; 	//Current mangopay card
		public $bank_id;	//Current mangopay bank

		public function __construct ( $wp_user = null, $autosave = false) {

			if ( $wp_user )
				$this -> wp_id = $wp_user->ID;

			if ( $wp_user -> mangopay_id )
				$this -> mangopay_id = $wp_user -> mangopay_id;

			if ( $wp_user -> bank_id )
				$this -> bank_id = $wp_user -> bank_id;

			if ( $wp_user -> card_id )
				$this -> card_id = $wp_user -> card_id;

			if ( $autosave ) 
				mwp_user::mwp_save_user ( $this );

			try {
				if ( $this -> mangopay_id )
					$mango_user = mwp_api::get_instance()->Users->Get($wp_user -> mangopay_id);

			} catch (m\ResponseException $e) {
				mwp_errors::mwp_manage_api_error ($e, true, 'mwp_user.mwp_construct');
			} catch (m\Exception $e) {
				mwp_errors::mwp_manage_api_error ($e, false, 'mwp_user.mwp_construct');
			}

			if ( $mango_user ) 
				self::mwp_set_values ( $mango_user );
			else 
				self::mwp_default_values( $wp_user ); 

		}

		public function mwp_default_values( $wp_user ) {

			if ( $wp_user ) {
				$this -> Email = $user -> user_email;
				$this-> FirstName = $user -> user_firstname;
				$this -> LastName = $user -> user_lastname;
			}

			$iso_code = substr ( get_locale(),  3, 2 );
			$this -> Nationality = $iso_code;
			$this -> CountryOfResidence = $iso_code;
			$this -> LegalRepresentativeNationality = $iso_code;
			$this -> LegalRepresentativeCountryOfResidence = $iso_code;
			$this -> LegalRepresentativeNationality = $iso_code;
			$this -> LegalRepresentativeCountryOfResidence = $iso_code;

		}

		public function mwp_set_values( $mango_user ) {

			if ( $mango_user ) {
				$this -> is_legal_user = get_class( $mango_user ) == 'MangoPay\UserLegal';
				$yFields =  mwp_get_fields ( ($this -> is_legal_user ? 'Legal' : 'Natural') );
				foreach ( $yFields as $field )
					$this -> $field = $mango_user -> $field;
			}

		}

		public function mwp_save ( $user = null ) {

			return mwp_user::mwp_save_user ( ($user ? $user : $this) );

		}

		static public function mwp_save_user ( $user ) {

			$api =  mwp_api::get_instance();

			//Is legal or natural user
			$is_legal_user = $_POST["user_type"] == "on";

			//Get fields names switching on user type
			$yFields = mwp_get_fields ( ( $is_legal_user ?  'Legal' : 'Natural' ) );

			//New or edit_user. If has changed is_legal_user, then create new object
			$is_edit = $user && $user -> mangopay_id && $is_legal_user == $user -> is_legal_user;

			//We don't need to save if there is no changes
			if ( $is_edit && ! mwp_has_changed_fields( $yFields, $user ) )
				return $this;

			try {
				//New mangopay user
				$mangopay_user = ( $is_legal_user ?  new m\UserLegal() : new m\UserNatural() );

				//Fill data
				foreach ( $yFields as $field ) 
					$mangopay_user -> $field = $_POST["mwp_{$field}"];

				//Convert Date fields
				self::user_convert_dates_to_number ( $mangopay_user, $is_legal_user );

				//Exec
				if ( $is_edit ) {
					$mangopay_user -> Id = $user -> mangopay_id;
					$result_user = $api -> Users -> Update ( $mangopay_user) ;
				} else {
					$result_user = $api -> Users -> Create ( $mangopay_user );
					$user -> mangopay_id = $result_user -> Id;
				}
				return $result_user;

			} catch (m\ResponseException $e) {
				mwp_errors::mwp_manage_api_error ($e, true, 'mwp_user.mwp_save');

			} catch (m\Exception $e) {
				mwp_errors::mwp_manage_api_error ($e, false, 'mwp_user.mwp_save');
			}
		}

		//Parse date to strtotime() seconds from Unix Epoch.
		function user_convert_dates_to_number ( &$user, $is_legal_user ) {

			if ( $is_legal_user ) {
				$user -> LegalRepresentativeBirthday = strtotime ( $user -> LegalRepresentativeBirthday );
			} else {
				$user -> Birthday = strtotime ( $user -> Birthday );
			}

		}

	}

?>
