<?php
	class mwp_forms {

		public function __construct() {
			require_once ( __DIR__ . '/mwp_fields.inc');
			require_once ( __DIR__ . '/mwp_print.inc');

			wp_enqueue_style( 'mwp_sc_contribute_css' );
			wp_enqueue_script( 'mwp_sc_contribute_js' );
		}

		private function f_user_section_header ( $defaults ) {

			return  mwp_print_tag ( "h3", 'mangopay_user_info' ) .
				mwp_print_table ( true ) .
					mwp_print_checkbox ( 'user_type', $defaults -> is_legal_user) .
				mwp_print_table ( false );

		}

		public function mwp_show_user_section ( $defaults ) {

			$output  = $this -> f_user_section_header ( $defaults, $defaults -> is_legal_user);
			$output .= mwp_print_tag ( "label", "[{$defaults -> mangopay_id}]" );
			$output .= mwp_print_tag ( "label", "[{$defaults -> is_legal_user}]" );
			$output .= mwp_print_section("mangopay_natural", $defaults, mwp_get_fields ( 'natural' ), !$defaults -> is_legal_user);
			$output .= mwp_print_section("mangopay_legal", $defaults, mwp_get_fields ( 'legal' ), $defaults -> is_legal_user);
			return $output;

		}

		public function mwp_show_payment_section ( $defaults ) {

			$output  .= mwp_print_section("mangopay_cards", $defaults, mwp_get_fields ( 'register' ), true);
			$output  .= mwp_print_section("mangopay_pay", $defaults, mwp_get_fields ( 'pay' ), true, "bt_pay");
			return $output;

		}

	}
?>
