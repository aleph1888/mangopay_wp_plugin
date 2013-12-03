<?php
//TODO configure global basedir.
require( dirname(dirname(dirname(dirname(dirname(__FILE__))))) . '/wp-load.php' );

$post_id = $_REQUEST['postid'];

$user = wp_get_current_user();
$post = get_post ($post_id );

$wallet_id = get_post_meta( "wallet_id", $post_id );
$beneficiary_id = get_the_author_meta( "mangopay_beneficiary_id", $user->ID );

$can_perform_action = current_user_can ( 'delete_others_posts' );
$can_perform_action = $can_perform_action || $user -> ID == $post -> post_author;
$can_perform_action = $can_perform_action && $beneficiary_id && $wallet_id;

//Get amount
$wallet_id = get_post_meta($post_id, "wallet_id", 1);
require_once (dirname(__FILE__) . "mangopay/lib/common.inc");
$wallet = request("wallets/$wallet_id", "GET");
$can_perform_action = $can_perform_action && ($wallet && $wallet -> Amount > 0);

if ( !$can_perform_action ) {
	wp_redirect( get_bloginfo('url'));
}

$body = json_encode(array(
		"UserID" => $user -> ID, 
		"WalletID" => $wallet_id, 
		"Amount" => $wallet -> Amount, 
		"BeneficiaryID" => $beneficiary_id
	));

$withdrawal = request("withdrawals", "POST", $body);

wp_redirect( get_edit_post_link ($post_id));



