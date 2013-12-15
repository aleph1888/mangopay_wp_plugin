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
	$user = new mwp\mwp_user ( wp_get_current_user() );	
	echo mwp\mwp_forms::mwp_show_user_section( $user );

	$user = new mwp\mwp_bank ( wp_get_current_user() );
	echo mwp\mwp_forms::mwp_show_bank_section( $user );

}

/** SAVE section **/
add_action( 'personal_options_update', 'mwp_save_profile_fields' );
add_action( 'edit_user_profile_update', 'mwp_save_profile_fields' );

function mwp_save_profile_fields( $user_id ) {

	//Gatekeeper
	if ( ! current_user_can( 'edit_user', $user_id ) )
		return false;

	//Save mangopay user
	$user = new mwp\mwp_user ( wp_get_current_user(), true );

	//Save mangopay bankaccount
	$bank = new mwp\mwp_bank ( wp_get_current_user(), true );

}

/** FUNCTIONS **/


//Show messages on profile box
//Catch some Titles with gettext filter to show error messages
add_filter( 'gettext', 'mwp_display_errors', 10, 3 );

function mwp_display_errors ($translated_text, $untranslated_text, $text){

	if ( $_SESSION["MWP_API_ERROR"] !=  NULL && ( $translated_text == 'Profile updated.' ||  $translated_text == 'User updated.') ) 
		return __ ( 'save_profile_error', 'mangopay_wp_plugin' ) . $_SESSION["MWP_API_ERROR"];

	return $translated_text;

}

