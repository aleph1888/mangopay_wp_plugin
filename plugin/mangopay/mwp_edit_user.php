<?php

/**
* New or Edit of a wordpress user on mangopay platform. 
* If New, saves mangopay id on user -> mangopay_id.
**/
function mwp_mangopay_edit_user( $user_id, $save_user = false, $save_beneficiary = false ) {

	require_once(dirname(__FILE__) . "/lib/common.inc");

	$user = get_userdata( $user_id );

	if ( $save_user ) {
		//Load values
		$parameters = array("Tag" => NULL,
				"FirstName" => $user->first_name,
				"LastName" => $user->second_name,
				"Email" => $user->email,
				"Nationality" => get_the_author_meta( 'nationality',  $user->ID),
				"PersonType" => NATURAL_PERSON, //  LEGAL_PERSONALITY,
				"CanRegisterMeanOfPayment" => 1, 
				"IP" => $_SERVER['REMOTE_ADDR'],
				"Birthday" => get_the_author_meta( 'birthday',  $user->ID),
				"Password" => NULL);

		// Convert format
		$body = json_encode($parameters);

		//NEW or EDIT
		$mangopay_user_id = get_the_author_meta( 'mangopay_id',  $user -> ID);
		if ($mangopay_user_id) {
			$mangopay_user= request("users/".$mangopay_user_id, "PUT", $body);
		} else {
			$mangopay_user = request("users", "POST", $body);
		}

		if( !isset($mangopay_user) || !isset($mangopay_user -> ID)) {
			print(" mwp_mangopay_edit_user: ERROR = Could not save mangopay user.");
		} elseif (!$mangopay_user_id) {
			//On NEW, save mangopay id on wordpress user object.
			update_usermeta( $user_id, 'mangopay_id', $mangopay_user -> ID );
		}
	}
	
	//Beneficiary object, only to get withdraws
	//Cannot update or delete this objects
	if ($save_beneficiary) {
		$parameters = array("BankAccountOwnerName" => get_the_author_meta( 'BankAccountOwnerName',  $user->ID),
				"BankAccountOwnerAddress" => get_the_author_meta( 'BankAccountOwnerAddress',  $user->ID), 
				"BankAccountIBAN" => get_the_author_meta( 'BankAccountIBAN',  $user->ID), 
				"BankAccountBIC" => get_the_author_meta( 'BankAccountBIC',  $user->ID), 
				"UserID" => $user->mangopay_id);

		// Convert format
		$body = json_encode($parameters);

		// execute request
		$beneficiary = request("beneficiaries", "POST", $body);

		if( isset($beneficiary) && isset($beneficiary->ID)) {
			update_usermeta( $user_id, 'mangopay_beneficiary_id', $beneficiary -> ID );
			return;
		}
	}

}
?>
