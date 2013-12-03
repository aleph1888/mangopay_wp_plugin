mangopay_wp_plugin
==================

Mangopay plugin for wordpress.

Descripción 
--------------
Simple plugin para wordpress. pasarela de pago Mangopay. Apartir del SDKv2 (LeetchiWalletServicesPHP-master.zip)

Acciones por ROL de USUARIO:
--------------
**Usuario >= author**
- Colocar shortcodes de donación relacionados con posts.
- Rellenar campos adicionales en profile, beneficiary.
- Solicitar withdraw de lo recolectado por un post.
**Usuario <= contributor**
- Rellenar campos adicionales en profile, contributor.
- Contribuir mediante calls al shortcode.

Shortcodes:
--------------
**Shortcode contribución**: [mwp_contribute amount="999" post_id="88"] 

![alt tag](https://raw.github.com/aleph1888/mangopay_wp_plugin/master/images/contribute_shortcode_0.jpg)

![alt tag](https://raw.github.com/aleph1888/mangopay_wp_plugin/master/images/contribute_shortcode_1.jpg)

- View: Botón de contribuir; input text cantidad.
- Params:
* Post_id => Si ausente, el contenedor del shortcode.
* Amount => Si ausente, se muestra un inputbox.

**Shortcode recogido**: [mwp_raised post_id="88"] 

![alt tag](https://raw.github.com/aleph1888/mangopay_wp_plugin/master/images/raised_shortcode.jpg)

- View: Total recogido por el proyecto. 
- Params:
* Post_id => Si ausente, el contenedor del shortcode.

Acciones:
--------------
- Rellenar campos adicionales en profile, beneficiary.
- Rellenar campos adicionales en profile, contributor.

![alt tag](https://raw.github.com/aleph1888/mangopay_wp_plugin/master/images/Profile-fields.jpg)

- Solicitar liquidación de lo recolectado por un post.

![alt tag](https://raw.github.com/aleph1888/mangopay_wp_plugin/master/images/Post-fields.jpg)

Internacionalización: 
--------------
/languages/

