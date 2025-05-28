include_guard()
message("microej/security component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/LLSEC_CIPHER_impl.c
    ${CMAKE_CURRENT_LIST_DIR}/src/LLSEC_DIGEST_impl.c
    ${CMAKE_CURRENT_LIST_DIR}/src/LLSEC_KEY_FACTORY_impl.c
    ${CMAKE_CURRENT_LIST_DIR}/src/LLSEC_KEY_PAIR_GENERATOR_impl.c
    ${CMAKE_CURRENT_LIST_DIR}/src/LLSEC_MAC_impl.c
    ${CMAKE_CURRENT_LIST_DIR}/src/LLSEC_PRIVATE_KEY_impl.c
    ${CMAKE_CURRENT_LIST_DIR}/src/LLSEC_PUBLIC_KEY_impl.c
    ${CMAKE_CURRENT_LIST_DIR}/src/LLSEC_RANDOM_impl.c
    ${CMAKE_CURRENT_LIST_DIR}/src/LLSEC_SIG_impl.c
    ${CMAKE_CURRENT_LIST_DIR}/src/LLSEC_X509_CERT_impl.c
)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE    ${CMAKE_CURRENT_LIST_DIR}/inc)
