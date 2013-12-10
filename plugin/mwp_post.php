<?php

/**
* Withdraw metabox in post edit sidebar.
**/
// User can withdraw if his role is author or up...
function mwp_current_user_can_withdraw () {
	$user = wp_get_current_user();
	return $user-> roles[0] != "suscriber" && $user-> roles[0] != "contributor";

}

add_action( 'submitpost_box', 'mwp_show_post_fields' );

function print_meta_box ( $post ) {
	
	//print previous messages (mwp_payout.php)
	echo $_SESSION["payout_result"];
	$_SESSION["payout_result"] = null;
	echo $_SESSION["MWP_API_ERROR"];

	//Search for wallet
	$wallet_id = get_post_meta( $post-> ID, "wallet_id", 1);
	if ( $wallet_id ) {
		require_once __DIR__ . "/includes/mwp_api.inc";
		$api = mwp_get_api();
		$wallet = $api->Wallets->Get($wallet_id);
	}

	//Display info
	if ( ! $wallet || $wallet->Balance->Amount == 0) {
		_e( "no_contributions", 'mangopay_wp_plugin');
	} else {
		
		//Total		
		echo __( "<h2>" . 'Total: ', 'mangopay_wp_plugin') . 
			 $wallet->Balance->Amount / 100 .
			 __( 'eur', 'mangopay_wp_plugin') . "</h2>\n";
		//Bankaccount
		require_once __DIR__ . "/includes/mwp_payout.inc";
		$payout = new mwp_payout;
		echo $payout -> mwp_bankaccount_get_info ( $post -> post_author );
	
		//Process url
		$params = "pid={$post->ID}";
		$url =  plugin_dir_url( __FILE__ ) . "mwp_payout.php?{$params}";

		//Submit a href
		$caption = __( 'withdraw', 'mangopay_wp_plugin');
		?> <br>
		<a href="<?php echo $url ?>" class="button"><?php echo $caption ?></a> <?php
	}
}

function mwp_show_post_fields( $post) { 
	$user = get_userdata( $post -> post_author );
	if ( wp_get_current_user() == $user )
		add_meta_box( $post->ID, __( "post_fields_title", 'mangopay_wp_plugin'), "print_meta_box", 'post', 'side', 'low', null);
}
