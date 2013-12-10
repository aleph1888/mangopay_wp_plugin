<?php 
	class mwp_user {
		
		private $api;
		public $Id;

		// Creates new user in mangopay from post data
		public function __construct ( $user = null ) {

			//Reset errors
			$_SESSION["MWP_API_ERROR"] = null;

			//Get SDK interface
			require_once __DIR__ . "/mwp_api.inc";
			$this -> api = mwp_get_api();

			require_once ( __DIR__ . "/mwp_fields.inc" );

			$this -> mwp_default_values ( $user );
		}

		private function user_manage_api_error ( $e, $details, $source ) {
			if ( $details ) {
				foreach ($e->GetErrorDetails() -> Errors as $error)
					$output .= $error . " ";

				$_SESSION["MWP_API_ERROR"] .= "[{$source}] {$output}"; 
			} else {
				$_SESSION["MWP_API_ERROR"] .= "[{$source}] {$e->GetMessage()}";
			}
			return;
		}

		public function mwp_default_values ( $user = null ) {

			if ( ! $user )
				$user = wp_get_current_user();

			if ( $user ) {
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

		}

		//Returns Mangopay Id only if new.
		public function mwp_save ( $user = null ) {

			//Is legal or natural user
			$is_legal_user = $_POST["user_type"] == "on";

			//Get fields names switching on user type
			$yFields = mwp_get_fields ( ( $is_legal_user ?  'legal' : 'natural' ) );

			//New or edit_user. If has changed is_legal_user, then create new object.
			$is_edit = $user && $user -> mangopay_id && $is_legal_user == $user -> is_legal_user;

			try {
				//New mangopay user
				$mangopay_user = ( $is_legal_user ?  new MangoPay\UserLegal() : new MangoPay\UserNatural() );

				//Fill data
				foreach ( $yFields as $field ) 
					$mangopay_user -> $field = $_POST["mwp_{$field}"];

				//Convert Date fields
				$this -> user_convert_dates_to_number ( $mangopay_user, $is_legal_user );

				if ( $is_edit ) {
					$mangopay_user -> Id = $user -> mangopay_id;
					$UserResult = $this -> api -> Users -> Update ( $mangopay_user) ;
					return null;
				} else {
					$UserResult = $this -> api -> Users -> Create ( $mangopay_user );
					$this -> Id = $UserResult -> Id;
					return $UserResult -> Id;
				}

				return $UserResult;

			} catch (MangoPay\ResponseException $e) {
				$this -> user_manage_api_error ($e, true, 'mwp_user.mwp_save');

			} catch (MangoPay\Exception $e) {
				$this -> user_manage_api_error ($e, false, 'mwp_user.mwp_save');
			}

		}

		//Parse date to strtotime() seconds from Unix Epoch.
		private function user_convert_dates_to_number ( &$user, $is_legal_user ) {

			if ( $is_legal_user ) {
				$user -> LegalRepresentativeBirthday = strtotime ( $user -> LegalRepresentativeBirthday );
			} else {
				$user -> Birthday = strtotime ( $user -> Birthday );
			}

		}

	}

?>
