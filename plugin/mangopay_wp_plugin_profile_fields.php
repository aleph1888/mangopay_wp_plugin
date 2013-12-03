<?php

/*
* Mangopay user object's fields:
*  Tag, FirstName, LastName, Email, Nationality,
*  PersonType (NATURAL_PERSON /  LEGAL_PERSONALITY), 
*  CanRegisterMeanOfPayment, IP, Birthday, Password);
*/

// User can withdraw if his role is author or up...
function current_user_can_withdraw () {
	$user = wp_get_current_user();
	return $user-> roles[0] != "suscriber" && $user-> roles[0] != "contributor";

}

//Return true or false wether any of the specified fields has change between post and object
function mwp_has_changed_fields ( $yFields, $user_id ) {
	foreach ( $yFields as $field ) {
		$has_changed = $_POST[$field] != get_the_author_meta( $field, $user_id );
		if ( $has_changed )
			return $has_changed;
	}
	return false;

}

//Print form-table (label, input) for yFields reading languange file (assume that for each field there is one field_description traduction);
function mwp_print_form ($title, $user_id, $yFields) {
	echo "<h3>" . __( $title, 'mangopay_wp_plugin') . "</h3>";
	echo "<table class='form-table'>";
		foreach ($yFields as $field) {
			echo "<tr>";
			echo "<th><label for='{$field}'>" . __( $field, 'mangopay_wp_plugin' ) . "</label></th>";
			echo "<td><input type='text' name='{$field}' id='{$field}' value='" . get_the_author_meta( $field, $user_id ) . "' class='regular-text' /><br />";
			echo "<span class='description'>" . __( $field . "_description", 'mangopay_wp_plugin') . "</span>";
			echo "</td>";
			echo "</tr>";
		}
	echo "</table>";

}

//SHOW actions
add_action( 'show_user_profile', 'mwp_show_profile_fields' );
add_action( 'edit_user_profile', 'mwp_show_profile_fields' );

function mwp_show_profile_fields( $user ) {
	mwp_print_form("profile_fields_title", $user -> ID, array ( "birthday", "nationality") );

	if ( current_user_can_withdraw () )
		mwp_print_form("beneficiary_fields_title", $user -> ID, array ( "BankAccountOwnerName", "BankAccountOwnerAddress", "BankAccountIBAN", "BankAccountBIC"));

}

//SAVE
add_action( 'personal_options_update', 'mwp_save_profile_fields' );
add_action( 'edit_user_profile_update', 'mwp_save_profile_fields' );

function mwp_save_profile_fields( $user_id ) {
	if ( !current_user_can( 'edit_user', $user_id ) )
		return false;

	$yFields = array ( "birthday", "nationality");	
	if ( $has_change_user = mwp_has_changed_fields ( $yFields, $user_id) ) {
		foreach ($yFields as $field) 
			update_user_meta( $user_id, $field, $_POST[$field] );
	}
	
	$yFields = array ( "BankAccountOwnerName", "BankAccountOwnerAddress", "BankAccountIBAN", "BankAccountBIC");
	if ( current_user_can_withdraw() && $has_change_beneficiary = mwp_has_changed_fields ( $yFields, $user_id)) {
		foreach ( $yFields as $field )
			update_user_meta( $user_id, $field, $_POST[$field] );
	}

	//Update mangopay entity
	require_once(dirname(__FILE__) . "/mangopay/mwp_edit_user.php");
	mwp_mangopay_edit_user ( $user_id, $has_change_user, $has_change_beneficiary );

}

