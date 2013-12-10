<?php
/**
* 
* Adds Mangopay user section at the end of user profile.
* Users that are author of posts, must create a Mangopay user before anybode can pay to these posts. Author is the owner of the wallet.
*
**/

/** SHOW section **/
add_action( 'show_user_profile', 'mwp_show_profile_fields' );
add_action( 'edit_user_profile', 'mwp_show_profile_fields' );

function mwp_show_profile_fields( $user ) {

	//Call script for user type selector checkbox
	wp_enqueue_script( 'user_type');

	//Print some errors before section. TODO make this smart
	echo "<br><div style='color:red'>{$_SESSION["MWP_API_ERROR"]}</div>";

	//Show form
	require_once ( __DIR__ . "/includes/mwp_forms.inc");
	echo  mwp_forms::mwp_show_user_section( $user );
	echo  mwp_forms::mwp_show_bank_section( $user );

}

/** SAVE section **/
add_action( 'personal_options_update', 'mwp_save_profile_fields' );
add_action( 'edit_user_profile_update', 'mwp_save_profile_fields' );

function mwp_save_profile_fields( $user_id ) {

	//Gatekeeper
	if ( ! current_user_can( 'edit_user', $user_id ) )
		return false;

	//Get type of user
	$is_legal_user = ( $_POST["user_type"] == "on" );

	//Get a list of field names switching on type of user
	require_once ( __DIR__ . '/includes/mwp_fields.inc');
	$yFields = mwp_get_fields ( ( $is_legal_user ? "legal" : "natural" ) );

	//Get user
	$user = get_userdata ( $user_id );

	//Save if need
	if ( mwp_has_changed_fields( $yFields, $user ) ) {
		//Update all fields
		foreach ($yFields as $field) 
			update_user_meta( $user_id, $field, $_POST["mwp_{$field}"] );

		//Save mangopay user
		require_once ( __DIR__ . "/includes/mwp_user.inc");
		$user_mangopay = new mwp_user;
		$new_mangopay_id = $user_mangopay -> mwp_save ( $user );

		//Only save Mangopay ID in Wordpress user if it is new and has created one
		if ( $new_mangopay_id ) 
			update_user_meta( $user_id, 'mangopay_id', $new_mangopay_id);

		//Update type of user
		update_user_meta( $user_id, 'is_legal_user', $is_legal_user );
	}

	$yFields = mwp_get_fields ( 'bank' );	
	if ( mwp_has_changed_fields( $yFields, $user ) ) {
		//Update all fields
		foreach ($yFields as $field) 
			update_user_meta( $user_id, $field, $_POST["mwp_{$field}"] );

		//Save mangopay bankaccount
		require_once ( __DIR__ . "/includes/mwp_payout.inc");
		$payout_mangopay = new mwp_payout;
		$new_bank_id = $payout_mangopay -> mwp_bankaccount_save ( $user );
		if ( $new_bank_id ) 
			update_user_meta( $user_id, 'bank_id', $new_bank_id);
		
	}

}

/** FUNCTIONS **/
//Return true or false wether any of the specified fields has change between post and object
function mwp_has_changed_fields ( $yFields, $user ) {

	foreach ( $yFields as $field ) {
		$has_changed = ( $_POST["mwp_{$field}"] != $user -> $field );
		if ( $has_changed )
			return true;
	}
	return false;

}

//Show messages on profile box
//Catch some Titles with gettext filter to show error messages
add_filter( 'gettext', 'mwp_display_errors', 10, 3 );

function mwp_display_errors ($translated_text, $untranslated_text, $text){

	if ( $_SESSION["MWP_API_ERROR"] !=  NULL && ( $translated_text == 'Profile updated.' ||  $translated_text == 'User updated.') ) 
		return __ ( 'save_profile_error', 'mangopay_wp_plugin' ) . $_SESSION["MWP_API_ERROR"];

	return $translated_text;

}

