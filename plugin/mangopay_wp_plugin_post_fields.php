<?php

/**
* Withdraw metabox in post edit sidebar.
**/

add_action( 'submitpost_box', 'mwp_show_post_fields' );

function print_meta_box ( $post ) {
	$user = get_userdata( $post -> post_author );

	if ( !(wp_get_current_user() != $user -> ID || current_user_can ( 'delete_others_posts' )) ) //Owner or admin restricted access 
		return;

	//Search for wallet
	$wallet_id = get_post_meta($post-> ID, "wallet_id", 1);
	if ($wallet_id) {
		require_once (dirname(__FILE__) . "/mangopay/lib/common.inc");
		$wallet = request("wallets/$wallet_id", "GET");
	}

	//Display info
	if (!$wallet || $wallet -> Amount == 0) {
		_e( "no_contributions", 'mangopay_wp_plugin');
	} elseif ( !isset( $user ->mangopay_beneficiary_id  ) || $user ->mangopay_beneficiary_id == 0 ) {
		_e( "must_fill_bankdata", 'mangopay_wp_plugin');
	} else {
		?>
		<table class="form-table">
			<tr>
				<th><label for="amount"> <?php _e( "total", 'mangopay_wp_plugin' ); ?></label></th>
				<td><?php echo ($wallet -> Amount . __( "eur", 'mangopay_wp_plugin' )); ?></td>
			</tr>
			<?php
				$yFields = array ( "BankAccountOwnerName", "BankAccountOwnerAddress", "BankAccountIBAN", "BankAccountBIC");
				foreach ( $yFields as $field ) { 
					echo "<tr><td><label>" . __( $field, 'mangopay_wp_plugin' ) . "</label></td>";
					echo "<td><label for='{$field}'>" .  get_the_author_meta( $field, $user -> ID ) . "</label></td></tr>";
				}
			?>
			<tr>
				<td> &nbsp; </td>
				<td>
					<?php 
					$url =  plugin_dir_url( __FILE__ ) . "/mangopay/mwp_withdraw.php?postid=" . $post-> ID ;
					$caption = __( 'withdraw', 'mangopay_wp_plugin');
					?>
					<a href="<?php echo $url ?>" class="button"><?php echo $caption ?></a>
				</td>
			</tr>
		</table>
		<?php
	}
}

function mwp_show_post_fields( $post) { 
	add_meta_box( $post->ID, __( "post_fields_title", 'mangopay_wp_plugin'), "print_meta_box", 'post', 'side', 'low', null);
}
