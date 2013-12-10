<?php
/**
* 
* Manage Mangopay user section in profile:
*
*	SHOW section 
*	SAVE section
* 	FUNCTIONS section 
**/

/** SHOW section **/

//add_action( 'show_user_profile', 'mwp_show_profile_fields' );
//add_action( 'edit_user_profile', 'mwp_show_profile_fields' );

require_once( dirname(dirname(dirname(__DIR__))). '/wp-load.php' );

mwp_forms_init ();

function mwp_forms_init () {
	get_header();
	mwp_show_forms();
	get_sidebar();
	get_footer(); 
}

function mwp_show_forms () {

	wp_enqueue_script ( "mwp_sc_contribute_js" );

	require_once ( __DIR__ . "/includes/mwp_forms.inc");
	$forms = new mwp_forms;

	require_once ( __DIR__ . "/includes/mwp_pay.inc");
	$po = new mwp_pay;
	$output .= '<div style="margin: 0 auto; width:900px">';
	$output .= "<form name='frmContribute' action='mwp_gateway.php' method='POST'>";
		//Display user data
		$output .= $forms -> mwp_show_user_section( $po );

		//Cards and paymentDirect
		$output .= $forms -> mwp_show_payment_section( $po );
		
		$output .=  "<input type='hidden' name ='mwp_post_id' value='{$_POST['post_id']}'>";
	$output .= "</form>";
	$output .= "</div>";
	echo $output;

}










