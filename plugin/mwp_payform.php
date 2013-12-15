<?php
/**
* 
* One step payform to Mangopay
*
**/

require_once( dirname(dirname(dirname(__DIR__))). '/wp-load.php' );
mwp_forms_init ();

function mwp_forms_init () {

	_wp_admin_bar_init();
	get_header();
	mwp_show_forms();
	get_sidebar();
	get_footer(); 

}

function mwp_show_forms () {

	require_once ( __DIR__ . "/includes/mwp_pay.inc");
	$po = new mwp\mwp_pay ( wp_get_current_user () );

	$output .= '<div style="margin: 0 auto; width:900px">';
	$output .= "<form name='frmContribute' action='mwp_gateway.php' method='POST'>";
		
		//Ask for register (if user don't, we don't keep information, but do the process)
		$output .= mwp\mwp_forms::mwp_show_wordpress_login();

		//Display user data
		$output .= mwp\mwp_forms::mwp_show_user_section( $po->user, ! $po->user->mangopay_id );

		//Cards and paymentDirect
		$output .= mwp\mwp_forms::mwp_show_payment_section( $po, ! $po->user->card_id );
		
		$output .=  "<input type='hidden' name ='mwp_post_id' value='{$_REQUEST['mwp_post_id']}'>";
	$output .= "</form>";
	$output .= "</div>";
	echo $output;

}

