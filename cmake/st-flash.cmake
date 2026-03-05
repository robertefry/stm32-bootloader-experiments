
find_program(STFLASH st-flash)

function(target_enable_st_flash target origin)

  add_custom_target(flash-${target}
    COMMAND ${STFLASH} write
      $<TARGET_FILE_DIR:${target}>/$<TARGET_FILE_BASE_NAME:${target}>.bin
      ${origin}
    DEPENDS $<TARGET_FILE:${target}>
    COMMENT "Flashing $<TARGET_FILE_BASE_NAME:${target}>.bin"
    VERBATIM
  )

endfunction()
