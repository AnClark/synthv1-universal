# project (${SYNTHV1_SRC_DIR}/synthv1)

set (SYNTHV1_SRC_DIR "${PROJECT_SOURCE_DIR}/src/src")

include_directories (
    ${SYNTHV1_SRC_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

if (EXISTS ${SYNTHV1_SRC_DIR}/config.h)
  file (REMOVE ${SYNTHV1_SRC_DIR}/config.h)
endif ()
configure_file (${SYNTHV1_SRC_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)
configure_file (config_dpf.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config_dpf.h)

set (HEADERS
  ${SYNTHV1_SRC_DIR}/synthv1.h
  ${SYNTHV1_SRC_DIR}/synthv1_config.h
  ${SYNTHV1_SRC_DIR}/synthv1_filter.h
  ${SYNTHV1_SRC_DIR}/synthv1_formant.h
  ${SYNTHV1_SRC_DIR}/synthv1_wave.h
  ${SYNTHV1_SRC_DIR}/synthv1_ramp.h
  ${SYNTHV1_SRC_DIR}/synthv1_list.h
  ${SYNTHV1_SRC_DIR}/synthv1_fx.h
  ${SYNTHV1_SRC_DIR}/synthv1_reverb.h
  synthv1_param.h       # Overridden by my modded version
  ${SYNTHV1_SRC_DIR}/synthv1_sched.h
  ${SYNTHV1_SRC_DIR}/synthv1_tuning.h
  ${SYNTHV1_SRC_DIR}/synthv1_programs.h
  ${SYNTHV1_SRC_DIR}/synthv1_controls.h
)

set (SOURCES
  ${SYNTHV1_SRC_DIR}/synthv1.cpp
  ${SYNTHV1_SRC_DIR}/synthv1_config.cpp
  ${SYNTHV1_SRC_DIR}/synthv1_formant.cpp
  ${SYNTHV1_SRC_DIR}/synthv1_wave.cpp
  synthv1_param.cpp     # Overridden by my modded version
  ${SYNTHV1_SRC_DIR}/synthv1_sched.cpp
  ${SYNTHV1_SRC_DIR}/synthv1_tuning.cpp
  ${SYNTHV1_SRC_DIR}/synthv1_programs.cpp
  ${SYNTHV1_SRC_DIR}/synthv1_controls.cpp
)


set (HEADERS_UI
  ${SYNTHV1_SRC_DIR}/synthv1_ui.h
  ${SYNTHV1_SRC_DIR}/synthv1widget.h
  ${SYNTHV1_SRC_DIR}/synthv1widget_env.h
  ${SYNTHV1_SRC_DIR}/synthv1widget_filt.h
  ${SYNTHV1_SRC_DIR}/synthv1widget_wave.h
  ${SYNTHV1_SRC_DIR}/synthv1widget_param.h
  ${SYNTHV1_SRC_DIR}/synthv1widget_keybd.h
  ${SYNTHV1_SRC_DIR}/synthv1widget_preset.h
  ${SYNTHV1_SRC_DIR}/synthv1widget_status.h
  ${SYNTHV1_SRC_DIR}/synthv1widget_programs.h
  ${SYNTHV1_SRC_DIR}/synthv1widget_controls.h
  ${SYNTHV1_SRC_DIR}/synthv1widget_control.h
  ${SYNTHV1_SRC_DIR}/synthv1widget_palette.h
  ${SYNTHV1_SRC_DIR}/synthv1widget_config.h
)

set (SOURCES_UI
  ${SYNTHV1_SRC_DIR}/synthv1_ui.cpp
  ${SYNTHV1_SRC_DIR}/synthv1widget.cpp
  ${SYNTHV1_SRC_DIR}/synthv1widget_env.cpp
  ${SYNTHV1_SRC_DIR}/synthv1widget_filt.cpp
  ${SYNTHV1_SRC_DIR}/synthv1widget_wave.cpp
  ${SYNTHV1_SRC_DIR}/synthv1widget_param.cpp
  ${SYNTHV1_SRC_DIR}/synthv1widget_keybd.cpp
  ${SYNTHV1_SRC_DIR}/synthv1widget_preset.cpp
  ${SYNTHV1_SRC_DIR}/synthv1widget_status.cpp
  ${SYNTHV1_SRC_DIR}/synthv1widget_programs.cpp
  ${SYNTHV1_SRC_DIR}/synthv1widget_controls.cpp
  ${SYNTHV1_SRC_DIR}/synthv1widget_control.cpp
  ${SYNTHV1_SRC_DIR}/synthv1widget_palette.cpp
  ${SYNTHV1_SRC_DIR}/synthv1widget_config.cpp
)

set (FORMS_UI
  ${SYNTHV1_SRC_DIR}/synthv1widget.ui
  ${SYNTHV1_SRC_DIR}/synthv1widget_control.ui
  ${SYNTHV1_SRC_DIR}/synthv1widget_palette.ui
  ${SYNTHV1_SRC_DIR}/synthv1widget_config.ui
)

set (RESOURCES_UI
  ${SYNTHV1_SRC_DIR}/synthv1.qrc
)


set (HEADERS_LV2
  ${SYNTHV1_SRC_DIR}/synthv1_lv2.h
  ${SYNTHV1_SRC_DIR}/synthv1_lv2ui.h
  ${SYNTHV1_SRC_DIR}/synthv1widget_lv2.h
)

set (SOURCES_LV2
  ${SYNTHV1_SRC_DIR}/synthv1_lv2.cpp
  ${SYNTHV1_SRC_DIR}/synthv1_lv2ui.cpp
  ${SYNTHV1_SRC_DIR}/synthv1widget_lv2.cpp
)


set (HEADERS_JACK
  ${SYNTHV1_SRC_DIR}/synthv1_nsm.h
  ${SYNTHV1_SRC_DIR}/synthv1_jack.h
  ${SYNTHV1_SRC_DIR}/synthv1widget_jack.h
)

set (SOURCES_JACK
  ${SYNTHV1_SRC_DIR}/synthv1_nsm.cpp
  ${SYNTHV1_SRC_DIR}/synthv1_jack.cpp
  ${SYNTHV1_SRC_DIR}/synthv1widget_jack.cpp
)


add_library (${PROJECT_NAME} STATIC
  ${HEADERS}
  ${SOURCES}
)

add_library (${PROJECT_NAME}_ui STATIC
  ${HEADERS_UI}
  ${SOURCES_UI}
  ${FORMS_UI}
  ${RESOURCES_UI}
)

if (CONFIG_DPF)
  set (DPF_PLUGIN_TYPES)
  if (CONFIG_DPF_VST2)
    list (APPEND DPF_PLUGIN_TYPES "vst2")
  endif ()
  if (CONFIG_DPF_VST3)
    list (APPEND DPF_PLUGIN_TYPES "vst3")
  endif ()
  if (CONFIG_DPF_CLAP)
    list (APPEND DPF_PLUGIN_TYPES "clap")
  endif ()
  if (CONFIG_DPF_LV2)
    list (APPEND DPF_PLUGIN_TYPES "lv2")
  endif ()

  dpf_add_plugin (${PROJECT_NAME}_dpf
    TARGETS ${DPF_PLUGIN_TYPES}
    UI_TYPE external
    FILES_DSP
      synthv1_dpf.cpp
    FILES_UI
      synthv1_dpfui.cpp
      synthv1widget_dpf.cpp
    MONOLITHIC
  )
endif ()

set_target_properties (${PROJECT_NAME}    PROPERTIES CXX_STANDARD 17)
set_target_properties (${PROJECT_NAME}_ui PROPERTIES CXX_STANDARD 17)

if (QT_VERSION VERSION_GREATER_EQUAL 6.0.0)
  target_compile_options (${PROJECT_NAME}    PRIVATE -fPIC)
  target_compile_options (${PROJECT_NAME}_ui PRIVATE -fPIC)
endif ()

# Add some debugger flags.
if (CONFIG_DEBUG AND UNIX AND NOT APPLE)
  set (CONFIG_DEBUG_OPTIONS -g -fsanitize=address -fno-omit-frame-pointer)
endif ()

target_link_libraries (${PROJECT_NAME}    PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Xml)
target_link_libraries (${PROJECT_NAME}_ui PUBLIC Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Svg ${PROJECT_NAME})

if (CONFIG_DPF)
  set_target_properties (${PROJECT_NAME}_dpf PROPERTIES CXX_STANDARD 17)
  set_target_properties (${PROJECT_NAME}_dpf-dsp PROPERTIES CXX_STANDARD 17)
  set_target_properties (${PROJECT_NAME}_dpf-ui PROPERTIES CXX_STANDARD 17)

  target_link_libraries (${PROJECT_NAME}_dpf PRIVATE ${PROJECT_NAME}_ui)

  target_include_directories (${PROJECT_NAME}_dpf-dsp PRIVATE ${Qt6Widgets_INCLUDE_DIRS})
  target_include_directories(${PROJECT_NAME}_dpf-ui PRIVATE ${Qt6Widgets_INCLUDE_DIRS})

  set (DPF_BINARY_DIR ${PROJECT_BINARY_DIR}/bin)

  if (UNIX AND NOT APPLE)
    if (CONFIG_PREFIX MATCHES $ENV{HOME})
      set (CONFIG_VSTDIR ${CONFIG_PREFIX}/.vst)
      set (CONFIG_VST3DIR ${CONFIG_PREFIX}/.vst3)
      set (CONFIG_CLAPDIR ${CONFIG_PREFIX}/.clap)
      set (CONFIG_LV2DIR ${CONFIG_PREFIX}/.lv2)
    else ()
      set (CONFIG_VSTDIR ${CONFIG_LIBDIR}/vst)
      set (CONFIG_VST3DIR ${CONFIG_LIBDIR}/vst3)
      set (CONFIG_CLAPDIR ${CONFIG_LIBDIR}/clap)
      set (CONFIG_LV2DIR ${CONFIG_LIBDIR}/lv2)
    endif ()

    set (DPF_VST2_BINARY ${DPF_BINARY_DIR}/${PROJECT_NAME}_dpf-vst2.so)
    set (DPF_CLAP_BINARY ${DPF_BINARY_DIR}/${PROJECT_NAME}_dpf.clap)
    set (DPF_LV2_BINARY ${DPF_BINARY_DIR}/${PROJECT_NAME}_dpf.lv2/${PROJECT_NAME}_dpf.so)
    dpf__determine_vst3_package_architecture(VST3_ARCH)
    set (DPF_VST3_BINARY ${DPF_BINARY_DIR}/${PROJECT_NAME}_dpf.vst3/Contents/${VST3_ARCH}-linux/${PROJECT_NAME}_dpf.so)

    set (VST2_SUFFIX .so)
    set (VST3_SUFFIX .so)
  elseif (WIN32)
    set (CONFIG_VSTDIR ${CONFIG_WINDOWS_VST_PATH})
    set (CONFIG_VST3DIR ${CONFIG_WINDOWS_VST3_PATH})
    set (CONFIG_CLAPDIR ${CONFIG_WINDOWS_CLAP_PATH})
    set (CONFIG_LV2DIR ${CONFIG_WINDOWS_LV2_PATH})

    set (DPF_VST2_BINARY ${DPF_BINARY_DIR}/${PROJECT_NAME}_dpf-vst2.dll)
    set (DPF_CLAP_BINARY ${DPF_BINARY_DIR}/${PROJECT_NAME}_dpf.clap)
    set (DPF_LV2_BINARY ${DPF_BINARY_DIR}/${PROJECT_NAME}_dpf.lv2/${PROJECT_NAME}_dpf.dll)
    dpf__determine_vst3_package_architecture(VST3_ARCH)
    set (DPF_VST3_BINARY ${DPF_BINARY_DIR}/${PROJECT_NAME}_dpf.vst3/Contents/${VST3_ARCH}-win/${PROJECT_NAME}_dpf.vst3)

    set (VST2_SUFFIX .dll)
    set (VST3_SUFFIX .vst3)
  endif ()

  if (NOT CONFIG_DEBUG)
    if (CONFIG_DPF_VST2)
      add_custom_command(TARGET ${PROJECT_NAME}_dpf-vst2  POST_BUILD
        COMMAND strip ${DPF_VST2_BINARY})
    endif ()
    if (CONFIG_DPF_VST3)
      add_custom_command(TARGET ${PROJECT_NAME}_dpf-vst3  POST_BUILD
        COMMAND strip ${DPF_VST3_BINARY})
    endif ()
    if (CONFIG_DPF_CLAP)
      add_custom_command(TARGET ${PROJECT_NAME}_dpf-clap  POST_BUILD
        COMMAND strip ${DPF_CLAP_BINARY})
    endif ()
    if (CONFIG_DPF_LV2)
      add_custom_command(TARGET ${PROJECT_NAME}_dpf-lv2  POST_BUILD
        COMMAND strip ${DPF_LV2_BINARY})
    endif ()
  endif ()

  if (CONFIG_DPF_VST2)
    install (FILES ${DPF_VST2_BINARY}
      RENAME ${PROJECT_NAME}-vst2${VST2_SUFFIX}
      DESTINATION ${CONFIG_VSTDIR})
  endif ()

  if (CONFIG_DPF_VST3)
    install (FILES ${DPF_VST3_BINARY}
      RENAME ${PROJECT_NAME}${VST3_SUFFIX}
      DESTINATION ${CONFIG_VST3DIR}/${PROJECT_NAME}.vst3/Contents/${VST3_ARCH}-linux)
  endif ()

  if (CONFIG_DPF_CLAP)
    install (FILES ${DPF_CLAP_BINARY}
      RENAME ${PROJECT_NAME}.clap
      DESTINATION ${CONFIG_CLAPDIR})
  endif ()

  if (CONFIG_DPF_LV2)   # Explicitly separated from the default LV2 build
    install (FILES ${DPF_LV2_BINARY}
      ${DPF_BINARY_DIR}/${PROJECT_NAME}_dpf.lv2/manifest.ttl
      ${DPF_BINARY_DIR}/${PROJECT_NAME}_dpf.lv2/${PROJECT_NAME}_dpf.ttl
      DESTINATION ${CONFIG_LV2DIR}/${PROJECT_NAME}_dpf.lv2)
  endif ()
endif ()
