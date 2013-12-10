<?php
	class mwp_forms {

		public function __construct() {
		}

		static private function f_user_section_header ( $defaults ) {

			return  mwp_print_tag ( "h3", 'mangopay_user_info' ) .
				mwp_print_table ( true ) .
					mwp_print_checkbox ( 'user_type', $defaults -> is_legal_user) .
				mwp_print_table ( false );

		}

		static function f_ini ( $css_js = false ) {
			require_once ( __DIR__ . '/mwp_fields.inc');
			require_once ( __DIR__ . '/mwp_print.inc');

			if (  $css_js ) {
				wp_enqueue_style( 'mwp_sc_contribute_css' );
				wp_enqueue_script( 'mwp_sc_contribute_js' );
			}
		}
	
		static function mwp_show_user_section ( $defaults ) {
			mwp_forms::f_ini  ( true );
			$output  = mwp_forms::f_user_section_header ( $defaults, $defaults -> is_legal_user);
			$output .= mwp_print_tag ( "label", "[{$defaults -> mangopay_id}]" );
			$output .= mwp_print_tag ( "label", "[{$defaults -> is_legal_user}]" );
			$output .= mwp_print_tag ( "label", "[{$defaults -> bank_id}]" );
			$output .= mwp_print_section("mangopay_natural", $defaults, mwp_get_fields ( 'natural' ), !$defaults -> is_legal_user);
			$output .= mwp_print_section("mangopay_legal", $defaults, mwp_get_fields ( 'legal' ), $defaults -> is_legal_user);
			return $output;

		}

		static function mwp_show_payment_section ( $defaults ) {
			mwp_forms::f_ini ();
			$output  .= mwp_print_section("mangopay_cards", $defaults, mwp_get_fields ( 'register' ), true);
			$output  .= mwp_print_section("mangopay_pay", $defaults, mwp_get_fields ( 'pay' ), true, __('bt_pay', 'mangopay_wp_plugin'));
			return $output;

		}

		static function mwp_show_bank_section ( $defaults ) {
			mwp_forms::f_ini ();
			$can_payout =  $defaults-> roles[0] != "suscriber" && $defaults-> roles[0] != "contributor";
			$output  .= mwp_print_section("mangopay_bank", $defaults, mwp_get_fields ( 'bank' ), $can_payout );
			return $output;

		}

		static function mwp_show_bank_section_info ( $defaults ) {
			mwp_forms::f_ini ();
			return mwp_print_section_info("mangopay_bank", $defaults, mwp_get_fields ( 'bank' ), true );
		}


	}
?>
