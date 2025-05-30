# config to select component, the format is CONFIG_USE_${component}
# Please refer to cmake files below to get available components:
#  ${SdkRootDirPath}/devices/MCXN947/all_lib_device.cmake

set(CONFIG_BOARD frdmmcxn947)
set(CONFIG_COMPILER gcc)
set(CONFIG_CORE cm33)
set(CONFIG_CORE_ID cm33_core0)
set(CONFIG_DEVICE MCXN947)
set(CONFIG_DEVICE_ID MCXN947)
set(CONFIG_DSP DSP)
set(CONFIG_FPU SP_FPU)
set(CONFIG_KIT frdmmcxn947)
set(CONFIG_TOOLCHAIN armgcc)
set(CONFIG_USE_CMSIS_Include_core_cm true)
set(CONFIG_USE_COMPONENT_CONFIGURATION false)
set(CONFIG_USE_component_gpio_adapter true)
set(CONFIG_USE_component_lists true)
set(CONFIG_USE_component_lpuart_adapter true)
set(CONFIG_USE_component_osa_free_rtos true)
set(CONFIG_USE_component_osa_interface true)
set(CONFIG_USE_component_reset_adapter true)
set(CONFIG_USE_component_serial_manager true)
set(CONFIG_USE_component_serial_manager_uart true)
set(CONFIG_USE_device_MCXN947_CMSIS true)
set(CONFIG_USE_device_MCXN947_startup true)
set(CONFIG_USE_device_MCXN947_system true)
set(CONFIG_USE_driver_clock true)
set(CONFIG_USE_driver_common true)
set(CONFIG_USE_driver_dbi true)
set(CONFIG_USE_driver_dbi_flexio_edma true)
set(CONFIG_USE_driver_dbi_flexio_smartdma true)
set(CONFIG_USE_driver_edma4 true)
set(CONFIG_USE_driver_edma_soc true)
set(CONFIG_USE_driver_flexio true)
set(CONFIG_USE_driver_flexio_mculcd true)
set(CONFIG_USE_driver_flexio_mculcd_edma true)
set(CONFIG_USE_driver_flexio_mculcd_smartdma true)
set(CONFIG_USE_driver_flexio_spi true)
set(CONFIG_USE_driver_ft5406_rt true)
set(CONFIG_USE_driver_gpio true)
set(CONFIG_USE_driver_gt911 true)
set(CONFIG_USE_driver_ili9341 true)
set(CONFIG_USE_driver_inputmux true)
set(CONFIG_USE_driver_inputmux_connections true)
set(CONFIG_USE_driver_irtc true)
set(CONFIG_USE_driver_lpc_smartdma true)
set(CONFIG_USE_driver_lpflexcomm true)
set(CONFIG_USE_driver_lpi2c true)
set(CONFIG_USE_driver_lpuart true)
set(CONFIG_USE_driver_mcx_spc true)
set(CONFIG_USE_driver_port true)
set(CONFIG_USE_driver_reset true)
set(CONFIG_USE_driver_ssd1963 true)
set(CONFIG_USE_driver_st7796s true)
set(CONFIG_USE_driver_usdhc true)
set(CONFIG_USE_middleware_fatfs true)
set(CONFIG_USE_middleware_fatfs_sd true)
set(CONFIG_USE_middleware_fatfs_template_sd true)
set(CONFIG_USE_middleware_freertos-kernel true)
set(CONFIG_USE_middleware_freertos-kernel_cm33_non_trustzone true)
set(CONFIG_USE_middleware_freertos-kernel_extension true)
set(CONFIG_USE_middleware_freertos-kernel_heap_3 true)
set(CONFIG_USE_middleware_freertos-kernel_template true)
#set(CONFIG_USE_middleware_lvgl true)
#set(CONFIG_USE_middleware_lvgl_demo_benchmark true)
#set(CONFIG_USE_middleware_lvgl_template true)
set(CONFIG_USE_middleware_sdmmc_common true)
set(CONFIG_USE_middleware_sdmmc_host_usdhc true)
set(CONFIG_USE_middleware_sdmmc_host_usdhc_cache true)
set(CONFIG_USE_middleware_sdmmc_host_usdhc_freertos true)
set(CONFIG_USE_middleware_sdmmc_osa_freertos true)
set(CONFIG_USE_middleware_sdmmc_sd true)
set(CONFIG_USE_middleware_sdmmc_usdhc_template true)
set(CONFIG_USE_utilities_misc_utilities true)
set(CONFIG_USE_utility_assert true)
set(CONFIG_USE_utility_assert_lite true)
set(CONFIG_USE_utility_debug_console true)
set(CONFIG_USE_utility_debug_console_lite true)
set(CONFIG_USE_utility_str true)

if (ENABLE_SHELL)
set(CONFIG_USE_utility_shell true)
unset(CONFIG_USE_utility_assert_lite)
unset(CONFIG_USE_utility_debug_console_lite)
endif(ENABLE_SHELL)

if (ENABLE_SEC)
set(CONFIG_USE_component_els_pkc true)
set(CONFIG_USE_component_els_pkc_aead true)
set(CONFIG_USE_component_els_pkc_aead_modes true)
set(CONFIG_USE_component_els_pkc_aes true)
set(CONFIG_USE_component_els_pkc_buffer true)
set(CONFIG_USE_component_els_pkc_cipher true)
set(CONFIG_USE_component_els_pkc_cipher_modes true)
set(CONFIG_USE_component_els_pkc_core true)
set(CONFIG_USE_component_els_pkc_data_integrity true)
set(CONFIG_USE_component_els_pkc_doc_mcxn true)
set(CONFIG_USE_component_els_pkc_ecc true)
set(CONFIG_USE_component_els_pkc_els true)
set(CONFIG_USE_component_els_pkc_els_common true)
set(CONFIG_USE_component_els_pkc_els_header_only true)
set(CONFIG_USE_component_els_pkc_flow_protection true)
set(CONFIG_USE_component_els_pkc_hash true)
set(CONFIG_USE_component_els_pkc_hashmodes true)
set(CONFIG_USE_component_els_pkc_hmac true)
set(CONFIG_USE_component_els_pkc_key true)
set(CONFIG_USE_component_els_pkc_mac true)
set(CONFIG_USE_component_els_pkc_mac_modes true)
set(CONFIG_USE_component_els_pkc_math true)
set(CONFIG_USE_component_els_pkc_memory true)
set(CONFIG_USE_component_els_pkc_padding true)
set(CONFIG_USE_component_els_pkc_param_integrity true)
set(CONFIG_USE_component_els_pkc_pkc true)
set(CONFIG_USE_component_els_pkc_platform_mcxn true)
set(CONFIG_USE_component_els_pkc_pre_processor true)
set(CONFIG_USE_component_els_pkc_prng true)
set(CONFIG_USE_component_els_pkc_random true)
set(CONFIG_USE_component_els_pkc_random_modes true)
set(CONFIG_USE_component_els_pkc_random_modes_ctr true)
set(CONFIG_USE_component_els_pkc_rsa true)
set(CONFIG_USE_component_els_pkc_secure_counter true)
set(CONFIG_USE_component_els_pkc_session true)
set(CONFIG_USE_component_els_pkc_standalone_keyManagement true)
set(CONFIG_USE_component_els_pkc_static_lib_mcxn true)
set(CONFIG_USE_component_els_pkc_toolchain true)
set(CONFIG_USE_component_els_pkc_trng true)
set(CONFIG_USE_component_els_pkc_trng_type_els true)
set(CONFIG_USE_middleware_mbedtls true)
set(CONFIG_USE_middleware_mbedtls_els_pkc_config true)
set(CONFIG_USE_middleware_mbedtls_port_els true)
set(CONFIG_USE_middleware_mbedtls_port_els_pkc true)
set(CONFIG_USE_middleware_mbedtls_rt2 true)
endif(ENABLE_SEC)

if (ENABLE_AI)
set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_binary true)
set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_headers true)
set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_neutron true)
set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_third_party_flatbuffers true)
set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_third_party_gemmlowp true)
set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_third_party_neutron_lib_binary true)
set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_third_party_ruy true)
endif(ENABLE_AI)

if (ENABLE_NET)
set(CONFIG_USE_component_silicon_id true)
set(CONFIG_USE_driver_flashiap true)
set(CONFIG_USE_driver_mcx_enet true)
set(CONFIG_USE_driver_phy-common true)
set(CONFIG_USE_driver_phy-device-lan8741 true)
set(CONFIG_USE_middleware_lwip true)
set(CONFIG_USE_middleware_lwip_contrib_ping true)
set(CONFIG_USE_middleware_lwip_mcx_ethernetif true)
set(CONFIG_USE_middleware_lwip_template true)
endif(ENABLE_NET)
