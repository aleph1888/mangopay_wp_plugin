<?php
session_start();
	require_once( dirname(dirname(dirname(__DIR__))). '/wp-load.php' );

	$post_id = $_GET['pid'];

	$user = get_userdata( get_post_field( 'post_author', $post_id ) );
	if ( wp_get_current_user()-> ID != $user -> ID )
		wp_redirect ( site_url () );

	$wallet_id = get_post_meta( $post_id, "wallet_id", 1);
	require_once __DIR__ . "/includes/mwp_api.inc";
	
	$wallet = mwp\mwp_api::get_instance()->Wallets->Get($wallet_id);

	require_once __DIR__ . "/includes/mwp_payout.inc";
	$payout = new mwp\mwp_payout;

	$_SESSION["payout_result"] = $payout -> mwp_do_payout ( 
				$user -> mangopay_id, 
				$wallet_id , 
				$wallet->Balance->Amount,
				$user->bank_id 
			);	
	wp_redirect ( admin_url ( "post.php?post={$post_id}&action=edit#mangopay_bank" ) );
?>
