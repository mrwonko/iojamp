cmake_minimum_required(VERSION 2.8)

project("iojamp")

option(InstallDir "Where to install the binaries to" "install")
option(UseOpenAL "Whether to use OpenAL" ON)
option(UseOpenALDlopen "When using OpenAL, whether to open via dlopen at runtime to be able to abort in case of absence" ON)
option(UseCURL "Whether to use curl" ON)
option(UseCURLDlopen "When using CURL, whether to open via dlopen at runtime to be able to abort in case of absence" ON)
option(UseCodecVorbis "Whether to use the Vorbis codec" OFF)
option(UseCodecOpus "Whether to use the Opus codec" ON)
option(UseMumble "Whether to enable Mumble support" ON)
option(UseVoip "Whether to enable Voice over IP" ON)
option(UseFreetype "Whether to use Freetype" OFF)
option(UseInternalSpeex "Whether to use the included Speex instead of a locally installed one" ON)
option(UseInternalOgg "Whether to use the included Ogg instead of a locally installed one" ON)
option(UseInternalOpus "Whether to use the included Opus instead of a locally installed one" ON)
option(UseInternalZlib "Whether to use the included ZLIB instead of a locally installed one" ON)
option(UseInternalJpeg "Whether to use the included libjpeg instead of a locally installed one" ON)
option(UseInternalPng "Whether to use the included PNG instead of a locally installed one" ON)
option(UseRendererDlopen "Whether to load the renderer via dlopen" ON)

set(Version 1.36)
set(CodeDir "${CMAKE_CURRENT_SOURCE_DIR}/code")
set(ClientDir "${CodeDir}/client")
set(ServerDir "${CodeDir}/server")
set(RendererCommonDir "${CodeDir}/renderercommon")
set(RendererGl1Dir "${CodeDir}/renderergl1")
set(RendererGl2Dir "${CodeDir}/renderergl2")
set(CommonDir "${CodeDir}/qcommon")
set(SdlDir "${CodeDir}/sdl")
set(AsmDir "${CodeDir}/asm")
set(SysDir "${CodeDir}/sys")
set(GameDir "${CodeDir}/game")
set(CGameDir "${CodeDir}/cgame")
set(BotlibDir "${CodeDir}/botlib")
set(NullDir "${CodeDir}/null")
set(UIDir "${CodeDir}/ui")
set(Q3UIDir "${CodeDir}/q3_ui")
set(JpegDir "${CodeDir}/jpeg-8c")
set(PngDir "${CodeDir}/png")
set(SpeexDir "${CodeDir}/libspeex")
set(OggDir "${CodeDir}/libogg-1.3.0")
set(OpusDir "${CodeDir}/opus-1.0.2")
set(OpusfileDir "${CodeDir}/opusfile-0.2")
set(ZlibDir "${CodeDir}/zlib")
set(Q3asmDir "${CodeDir}/tools/asm")
set(LburgDir "${CodeDir}/tools/lcc/lburg")
set(Q3cppDir "${CodeDir}/tools/lcc/cpp")
set(Q3lccEtcDir "${CodeDir}/tools/lcc/etc")
set(Q3lccSrcDir "${CodeDir}/tools/lcc/src")
set(SdlHeaderDir "${CodeDir}/SDL12")
set(LibsDir "${CodeDir}/libs")

# Names for binaries
set(RendererGl1Name "renderergl1")
set(RendererGl2Name "renderergl2")
set(IojampName "iojamp")


# CMake's FindCURL module sucks. I've improved it:
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules" ${CMAKE_MODULE_PATH})


#   Windows Setup
if(WIN32)
	# in the absence of wspiapi.h require Windows XP or later
	include(CheckIncludeFiles) #contains check_include_files()
	check_include_files(wspiapi.h HAVE_WSPIAPI_H)
	if(NOT HAVE_WSPIAPI_H)
		add_definitions(-DWINVER=0x501)
	endif(NOT HAVE_WSPIAPI_H)
endif(WIN32)

#   Common Setup

add_definitions(-DPRODUCT_VERSION=\"${Version}\")

find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)

set(CommonDefines "")# NO_ASM_SUPPORTED)

#   Common Renderer Setup

set(CommonRendererSrcFiles
	"${RendererCommonDir}/tr_font.c"
	"${RendererCommonDir}/tr_image_bmp.c"
	"${RendererCommonDir}/tr_image_jpg.c"
	"${RendererCommonDir}/tr_image_pcx.c"
	"${RendererCommonDir}/tr_image_png.c"
	"${RendererCommonDir}/tr_image_tga.c"
	"${RendererCommonDir}/tr_noise.c"
	"${RendererCommonDir}/tr_font.c"
	)
set(CommonRendererIncludeDirs ${SDL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
set(CommonRendererLibs ${SDL_LIBRARY} ${OPENGL_LIBRARIES})
set(CommonRendererDefines ${CommonDefines})

# FreeType
if(UseFreetype)
	set(CommonRendererDefines ${CommonRendererDefines} BUILD_FREETYPE)
	find_package(Freetype REQUIRED)
	set(CommonRendererIncludeDirs ${CommonRendererIncludeDirs} ${FREETYPE_INCLUDE_DIRS})
	set(CommonRendererLibs ${CommonRendererLibs} ${FREETYPE_LIBRARIES})
endif(UseFreetype)

# JPEG
if(UseInternalJpeg)
	set(CommonRendererDefines ${CommonRendererDefines} USE_INTERNAL_JPEG)
	set(CommonRendererIncludeDirs ${CommonRendererIncludeDirs} ${JpegDir})
	set(CommonRendererSrcFiles
		${CommonRendererSrcFiles}
		"${JpegDir}/jaricom.c"
		"${JpegDir}/jcapimin.c"
		"${JpegDir}/jcapistd.c"
		"${JpegDir}/jcarith.c"
		"${JpegDir}/jccoefct.c"
		"${JpegDir}/jccolor.c"
		"${JpegDir}/jcdctmgr.c"
		"${JpegDir}/jchuff.c"
		"${JpegDir}/jcinit.c"
		"${JpegDir}/jcmainct.c"
		"${JpegDir}/jcmarker.c"
		"${JpegDir}/jcmaster.c"
		"${JpegDir}/jcomapi.c"
		"${JpegDir}/jcparam.c"
		"${JpegDir}/jcprepct.c"
		"${JpegDir}/jcsample.c"
		"${JpegDir}/jctrans.c"
		"${JpegDir}/jdapimin.c"
		"${JpegDir}/jdapistd.c"
		"${JpegDir}/jdarith.c"
		"${JpegDir}/jdatadst.c"
		"${JpegDir}/jdatasrc.c"
		"${JpegDir}/jdcoefct.c"
		"${JpegDir}/jdcolor.c"
		"${JpegDir}/jddctmgr.c"
		"${JpegDir}/jdhuff.c"
		"${JpegDir}/jdinput.c"
		"${JpegDir}/jdmainct.c"
		"${JpegDir}/jdmarker.c"
		"${JpegDir}/jdmaster.c"
		"${JpegDir}/jdmerge.c"
		"${JpegDir}/jdpostct.c"
		"${JpegDir}/jdsample.c"
		"${JpegDir}/jdtrans.c"
		"${JpegDir}/jerror.c"
		"${JpegDir}/jfdctflt.c"
		"${JpegDir}/jfdctfst.c"
		"${JpegDir}/jfdctint.c"
		"${JpegDir}/jidctflt.c"
		"${JpegDir}/jidctfst.c"
		"${JpegDir}/jidctint.c"
		"${JpegDir}/jmemmgr.c"
		"${JpegDir}/jmemnobs.c"
		"${JpegDir}/jquant1.c"
		"${JpegDir}/jquant2.c"
		"${JpegDir}/jutils.c"
		)
else(UseInternalJpeg)
	find_package(JPEG REQUIRED)
	set(CommonRendererIncludeDirs ${CommonRendererIncludeDirs} ${JPEG_INCLUDE_DIR})
	set(CommonRendererLibs ${CommonRendererLibs} ${JPEG_LIBRARIES})
endif(UseInternalJpeg)

# PNG
if(UseInternalPng)
	set(CommonRendererIncludeDirs ${CommonRendererIncludeDirs} ${JpegDir})
	set(CommonRendererSrcFiles
		${CommonRendererSrcFiles}
		"${PngDir}/png.c"
		"${PngDir}/pngerror.c"
		"${PngDir}/pngget.c"
		"${PngDir}/pngmem.c"
		"${PngDir}/pngpread.c"
		"${PngDir}/pngread.c"
		"${PngDir}/pngrio.c"
		"${PngDir}/pngrtran.c"
		"${PngDir}/pngrutil.c"
		"${PngDir}/pngset.c"
		"${PngDir}/pngtrans.c"
		"${PngDir}/pngwio.c"
		"${PngDir}/pngwrite.c"
		"${PngDir}/pngwtran.c"
		"${PngDir}/pngwutil.c"
		)
else(UseInternalPng)
	find_package(PNG REQUIRED)
	set(CommonRendererDefines ${CommonRendererDefines} ${PNG_DEFINITIONS}) #not actually used, but let
	set(CommonRendererIncludeDirs ${CommonRendererIncludeDirs} ${PNG_INCLUDE_DIR})
	set(CommonRendererLibs ${CommonRendererLibs} ${PNG_LIBRARIES})
endif(UseInternalPng)

# When not linking into the executable, these files are required since functions from there are used
if(UseRendererDlopen)
	set(CommonRendererSrcFiles
		${CommonRendererSrcFiles}
		"${CommonDir}/q_shared.c"
		"${CommonDir}/puff.c"
		"${CommonDir}/q_math.c"
		)
endif(UseRendererDlopen)

#   Renderer GL 1

set(RendererGl1SrcFiles
	${CommonRendererSrcFiles}
	"${RendererGl1Dir}/tr_animation.c"
	"${RendererGl1Dir}/tr_backend.c"
	"${RendererGl1Dir}/tr_bsp.c"
	"${RendererGl1Dir}/tr_cmds.c"
	"${RendererGl1Dir}/tr_curve.c"
	"${RendererGl1Dir}/tr_flares.c"
	"${RendererGl1Dir}/tr_image.c"
	"${RendererGl1Dir}/tr_init.c"
	"${RendererGl1Dir}/tr_light.c"
	"${RendererGl1Dir}/tr_main.c"
	"${RendererGl1Dir}/tr_marks.c"
	"${RendererGl1Dir}/tr_mesh.c"
	"${RendererGl1Dir}/tr_model.c"
	"${RendererGl1Dir}/tr_model_iqm.c"
	"${RendererGl1Dir}/tr_scene.c"
	"${RendererGl1Dir}/tr_shade.c"
	"${RendererGl1Dir}/tr_shade_calc.c"
	"${RendererGl1Dir}/tr_shader.c"
	"${RendererGl1Dir}/tr_sky.c"
	"${RendererGl1Dir}/tr_shadows.c"
	"${RendererGl1Dir}/tr_surface.c"
	"${RendererGl1Dir}/tr_world.c"
	
	"${SdlDir}/sdl_gamma.c"
	"${SdlDir}/sdl_glimp.c"
	)
set(RendererGl1IncludeDirs ${CommonRendererIncludeDirs})
set(RendererGl1Libs ${CommonRendererLibs})
set(RendererGl1Defines ${CommonRendererDefines})

if(UseRendererDlopen)
	add_library(${RendererGl1Name} SHARED ${RendererGl1SrcFiles})
	target_link_libraries(${RendererGl1Name} ${RendererGl1Libs})
	set_target_properties(${RendererGl1Name} PROPERTIES COMPILE_DEFINITIONS ${RendererGl1Defines})
	set_target_properties(${RendererGl1Name} PROPERTIES INCLUDE_DIRECTORIES "${RendererGl1IncludeDirs}")
endif(UseRendererDlopen)

#   (Not supporting Renderer GL2 at the moment)

#   Executable

#ENABLE_LANGUAGE(ASM_MASM) #TODO: does this work for assembler on linux, too?

set(IojampSrcFiles
	"${ClientDir}/cl_cgame.c"
	"${ClientDir}/cl_cin.c"
	"${ClientDir}/cl_console.c"
	"${ClientDir}/cl_input.c"
	"${ClientDir}/cl_keys.c"
	"${ClientDir}/cl_main.c"
	"${ClientDir}/cl_net_chan.c"
	"${ClientDir}/cl_parse.c"
	"${ClientDir}/cl_scrn.c"
	"${ClientDir}/cl_ui.c"
	"${ClientDir}/cl_avi.c"
	
	"${CommonDir}/cm_load.c"
	"${CommonDir}/cm_patch.c"
	"${CommonDir}/cm_polylib.c"
	"${CommonDir}/cm_test.c"
	"${CommonDir}/cm_trace.c"
	
	"${CommonDir}/cmd.c"
	"${CommonDir}/common.c"
	"${CommonDir}/cvar.c"
	"${CommonDir}/files.c"
	"${CommonDir}/md4.c"
	"${CommonDir}/md5.c"
	"${CommonDir}/msg.c"
	"${CommonDir}/net_chan.c"
	"${CommonDir}/net_ip.c"
	"${CommonDir}/huffman.c"
	
	"${ClientDir}/snd_adpcm.c"
	"${ClientDir}/snd_dma.c"
	"${ClientDir}/snd_mem.c"
	"${ClientDir}/snd_mix.c"
	"${ClientDir}/snd_wavelet.c"
	
	"${ClientDir}/snd_main.c"
	"${ClientDir}/snd_codec.c"
	"${ClientDir}/snd_codec_wav.c"
	"${ClientDir}/snd_codec_ogg.c"
	"${ClientDir}/snd_codec_opus.c"
	
	"${ClientDir}/qal.c"
	"${ClientDir}/snd_openal.c"
	
	"${ClientDir}/cl_curl.c"
	
	"${ServerDir}/sv_bot.c"
	"${ServerDir}/sv_ccmds.c"
	"${ServerDir}/sv_client.c"
	"${ServerDir}/sv_game.c"
	"${ServerDir}/sv_init.c"
	"${ServerDir}/sv_main.c"
	"${ServerDir}/sv_net_chan.c"
	"${ServerDir}/sv_snapshot.c"
	"${ServerDir}/sv_world.c"
	
	"${CommonDir}/q_math.c"
	"${CommonDir}/q_shared.c"
	
	"${CommonDir}/unzip.c"
	"${CommonDir}/ioapi.c"
	"${CommonDir}/puff.c"
	"${CommonDir}/vm.c"
	"${CommonDir}/vm_interpreted.c"
	"${CommonDir}/vm_x86.c" #no VM support atm
	
	"${SdlDir}/sdl_input.c"
	"${SdlDir}/sdl_snd.c"
	
	#"${AsmDir}/snd_mixa.s"
	#"${AsmDir}/matha.s"
	#"${AsmDir}/snapvector.asm"
	#"${AsmDir}/ftola.asm"
	"${AsmDir}/ftola.c"
	"${AsmDir}/snapvector.c"
	
	"${SysDir}/con_log.c"
	"${SysDir}/sys_main.c"
	)

# Botlib needs BOTLIB define when building
set(IojampBotlibSrcFiles
	"${BotlibDir}/be_aas_bspq3.c"
	"${BotlibDir}/be_aas_cluster.c"
	"${BotlibDir}/be_aas_debug.c"
	"${BotlibDir}/be_aas_entity.c"
	"${BotlibDir}/be_aas_file.c"
	"${BotlibDir}/be_aas_main.c"
	"${BotlibDir}/be_aas_move.c"
	"${BotlibDir}/be_aas_optimize.c"
	"${BotlibDir}/be_aas_reach.c"
	"${BotlibDir}/be_aas_route.c"
	"${BotlibDir}/be_aas_routealt.c"
	"${BotlibDir}/be_aas_sample.c"
	"${BotlibDir}/be_ai_char.c"
	"${BotlibDir}/be_ai_chat.c"
	"${BotlibDir}/be_ai_gen.c"
	"${BotlibDir}/be_ai_goal.c"
	"${BotlibDir}/be_ai_move.c"
	"${BotlibDir}/be_ai_weap.c"
	"${BotlibDir}/be_ai_weight.c"
	"${BotlibDir}/be_ea.c"
	"${BotlibDir}/be_interface.c"
	"${BotlibDir}/l_crc.c"
	"${BotlibDir}/l_libvar.c"
	"${BotlibDir}/l_log.c"
	"${BotlibDir}/l_memory.c"
	"${BotlibDir}/l_precomp.c"
	"${BotlibDir}/l_script.c"
	"${BotlibDir}/l_struct.c"
	)
set_source_files_properties(${IojampBotlibSrcFiles} PROPERTIES COMPILE_DEFINITIONS "BOTLIB")
set(IojampSrcFiles ${IojampSrcFiles} ${IojampBotlibSrcFiles})
set(IojampLibs "")
set(IojampDefines ${CommonDefines}) # NO_VM_COMPILED)
set(IojampIncludeDirs "")

if(WIN32)
	set(IojampSrcFiles
		${IojampSrcFiles}
		"${SysDir}/con_passive.c"
		"${SysDir}/win_resource.rc"
		"${SysDir}/sys_win32.c"
		)
else(WIN32)
	set(IojampSrcFiles
		${IojampSrcFiles}
		"${SysDir}/con_tty.c"
		"${SysDir}/sys_unix.c"
		)
endif(WIN32)

# handle iojamp options

# OpenAL
if(UseOpenAL)
	set(IojampDefines ${IojampDefines} USE_OPENAL)
	find_package(OpenAL REQUIRED)
	set(IojampIncludeDirs ${IojampIncludeDirs} ${OPENAL_INCLUDE_DIR})
	if(UseOpenALDlopen)
		set(IojampDefines ${IojampDefines} USE_OPENAL_DLOPEN)
	else(UseOpenALDlopen)
		set(IojampLibs ${IojampLibs} ${OPENAL_LIBRARY})
	endif(UseOpenALDlopen)
endif(UseOpenAL)

# CURL
if(UseCURL)
	set(IojampDefines ${IojampDefines} USE_CURL)
	find_package(CURL REQUIRED)
	set(IojampIncludeDirs ${IojampIncludeDirs} ${CURL_INCLUDE_DIRS})
	if(UseCURLDlopen)
		set(IojampDefines ${IojampDefines} USE_CURL_DLOPEN)
	else(UseCURLDlopen)
		set(IojampLibs ${IojampLibs} ${CURL_LIBRARIES})
	endif(UseCURLDlopen)
endif(UseCURL)

# Vorbis
if(UseCodecVorbis)
	set(IojampDefines ${IojampDefines} USE_CODEC_VORBIS)
	# no FindVorbis module available, so let's just hope the user has his build environment set up correctly
	set(IojampLibs ${IojampLibs} vorbisfile vorbis)
	# Vorbis needs OGG
	set(NeedOgg ON)
endif(UseCodecVorbis)

# Opus
if(UseCodecOpus)
	set(IojampDefines ${IojampDefines} USE_CODEC_OPUS)
	if(UseInternalOpus)
		set(IojampDefines ${IojampDefines} OPUS_BUILD HAVE_LRINTF FLOATING_POINT USE_ALLOCA)
		set(IojampIncludeDirs ${IojampIncludeDirs} "${OpusDir}/include" "${OpusDir}/celt" "${OpusDir}/silk" "${OpusDir}/silk/float" "${OpusfileDir}/include")
		set(IojampSrcFiles
			${IojampSrcFiles}
			"${OpusDir}/src/opus.c"
			"${OpusDir}/src/opus_decoder.c"
			"${OpusDir}/src/opus_encoder.c"
			"${OpusDir}/src/opus_multistream.c"
			"${OpusDir}/src/repacketizer.c"
			
			"${OpusDir}/celt/bands.c"
			"${OpusDir}/celt/celt.c"
			"${OpusDir}/celt/cwrs.c"
			"${OpusDir}/celt/entcode.c"
			"${OpusDir}/celt/entdec.c"
			"${OpusDir}/celt/entenc.c"
			"${OpusDir}/celt/kiss_fft.c"
			"${OpusDir}/celt/laplace.c"
			"${OpusDir}/celt/mathops.c"
			"${OpusDir}/celt/mdct.c"
			"${OpusDir}/celt/modes.c"
			"${OpusDir}/celt/pitch.c"
			"${OpusDir}/celt/celt_lpc.c"
			"${OpusDir}/celt/quant_bands.c"
			"${OpusDir}/celt/rate.c"
			"${OpusDir}/celt/vq.c"
			
			"${OpusDir}/silk/CNG.c"
			"${OpusDir}/silk/code_signs.c"
			"${OpusDir}/silk/init_decoder.c"
			"${OpusDir}/silk/decode_core.c"
			"${OpusDir}/silk/decode_frame.c"
			"${OpusDir}/silk/decode_parameters.c"
			"${OpusDir}/silk/decode_indices.c"
			"${OpusDir}/silk/decode_pulses.c"
			"${OpusDir}/silk/decoder_set_fs.c"
			"${OpusDir}/silk/dec_API.c"
			"${OpusDir}/silk/enc_API.c"
			"${OpusDir}/silk/encode_indices.c"
			"${OpusDir}/silk/encode_pulses.c"
			"${OpusDir}/silk/gain_quant.c"
			"${OpusDir}/silk/interpolate.c"
			"${OpusDir}/silk/LP_variable_cutoff.c"
			"${OpusDir}/silk/NLSF_decode.c"
			"${OpusDir}/silk/NSQ.c"
			"${OpusDir}/silk/NSQ_del_dec.c"
			"${OpusDir}/silk/PLC.c"
			"${OpusDir}/silk/shell_coder.c"
			"${OpusDir}/silk/tables_gain.c"
			"${OpusDir}/silk/tables_LTP.c"
			"${OpusDir}/silk/tables_NLSF_CB_NB_MB.c"
			"${OpusDir}/silk/tables_NLSF_CB_WB.c"
			"${OpusDir}/silk/tables_other.c"
			"${OpusDir}/silk/tables_pitch_lag.c"
			"${OpusDir}/silk/tables_pulses_per_block.c"
			"${OpusDir}/silk/VAD.c"
			"${OpusDir}/silk/control_audio_bandwidth.c"
			"${OpusDir}/silk/quant_LTP_gains.c"
			"${OpusDir}/silk/VQ_WMat_EC.c"
			"${OpusDir}/silk/HP_variable_cutoff.c"
			"${OpusDir}/silk/NLSF_encode.c"
			"${OpusDir}/silk/NLSF_VQ.c"
			"${OpusDir}/silk/NLSF_unpack.c"
			"${OpusDir}/silk/NLSF_del_dec_quant.c"
			"${OpusDir}/silk/process_NLSFs.c"
			"${OpusDir}/silk/stereo_LR_to_MS.c"
			"${OpusDir}/silk/stereo_MS_to_LR.c"
			"${OpusDir}/silk/check_control_input.c"
			"${OpusDir}/silk/control_SNR.c"
			"${OpusDir}/silk/init_encoder.c"
			"${OpusDir}/silk/control_codec.c"
			"${OpusDir}/silk/A2NLSF.c"
			"${OpusDir}/silk/ana_filt_bank_1.c"
			"${OpusDir}/silk/biquad_alt.c"
			"${OpusDir}/silk/bwexpander_32.c"
			"${OpusDir}/silk/bwexpander.c"
			"${OpusDir}/silk/debug.c"
			"${OpusDir}/silk/decode_pitch.c"
			"${OpusDir}/silk/inner_prod_aligned.c"
			"${OpusDir}/silk/lin2log.c"
			"${OpusDir}/silk/log2lin.c"
			"${OpusDir}/silk/LPC_analysis_filter.c"
			"${OpusDir}/silk/LPC_inv_pred_gain.c"
			"${OpusDir}/silk/table_LSF_cos.c"
			"${OpusDir}/silk/NLSF2A.c"
			"${OpusDir}/silk/NLSF_stabilize.c"
			"${OpusDir}/silk/NLSF_VQ_weights_laroia.c"
			"${OpusDir}/silk/pitch_est_tables.c"
			"${OpusDir}/silk/resampler.c"
			"${OpusDir}/silk/resampler_down2_3.c"
			"${OpusDir}/silk/resampler_down2.c"
			"${OpusDir}/silk/resampler_private_AR2.c"
			"${OpusDir}/silk/resampler_private_down_FIR.c"
			"${OpusDir}/silk/resampler_private_IIR_FIR.c"
			"${OpusDir}/silk/resampler_private_up2_HQ.c"
			"${OpusDir}/silk/resampler_rom.c"
			"${OpusDir}/silk/sigm_Q15.c"
			"${OpusDir}/silk/sort.c"
			"${OpusDir}/silk/sum_sqr_shift.c"
			"${OpusDir}/silk/stereo_decode_pred.c"
			"${OpusDir}/silk/stereo_encode_pred.c"
			"${OpusDir}/silk/stereo_find_predictor.c"
			"${OpusDir}/silk/stereo_quant_pred.c"
			
			"${OpusDir}/silk/float/apply_sine_window_FLP.c"
			"${OpusDir}/silk/float/corrMatrix_FLP.c"
			"${OpusDir}/silk/float/encode_frame_FLP.c"
			"${OpusDir}/silk/float/find_LPC_FLP.c"
			"${OpusDir}/silk/float/find_LTP_FLP.c"
			"${OpusDir}/silk/float/find_pitch_lags_FLP.c"
			"${OpusDir}/silk/float/find_pred_coefs_FLP.c"
			"${OpusDir}/silk/float/LPC_analysis_filter_FLP.c"
			"${OpusDir}/silk/float/LTP_analysis_filter_FLP.c"
			"${OpusDir}/silk/float/LTP_scale_ctrl_FLP.c"
			"${OpusDir}/silk/float/noise_shape_analysis_FLP.c"
			"${OpusDir}/silk/float/prefilter_FLP.c"
			"${OpusDir}/silk/float/process_gains_FLP.c"
			"${OpusDir}/silk/float/regularize_correlations_FLP.c"
			"${OpusDir}/silk/float/residual_energy_FLP.c"
			"${OpusDir}/silk/float/solve_LS_FLP.c"
			"${OpusDir}/silk/float/warped_autocorrelation_FLP.c"
			"${OpusDir}/silk/float/wrappers_FLP.c"
			"${OpusDir}/silk/float/autocorrelation_FLP.c"
			"${OpusDir}/silk/float/burg_modified_FLP.c"
			"${OpusDir}/silk/float/bwexpander_FLP.c"
			"${OpusDir}/silk/float/energy_FLP.c"
			"${OpusDir}/silk/float/inner_product_FLP.c"
			"${OpusDir}/silk/float/k2a_FLP.c"
			"${OpusDir}/silk/float/levinsondurbin_FLP.c"
			"${OpusDir}/silk/float/LPC_inv_pred_gain_FLP.c"
			"${OpusDir}/silk/float/pitch_analysis_core_FLP.c"
			"${OpusDir}/silk/float/scale_copy_vector_FLP.c"
			"${OpusDir}/silk/float/scale_vector_FLP.c"
			"${OpusDir}/silk/float/schur_FLP.c"
			"${OpusDir}/silk/float/sort_FLP.c"
			
			"${OpusfileDir}/src/http.c"
			"${OpusfileDir}/src/info.c"
			"${OpusfileDir}/src/internal.c"
			"${OpusfileDir}/src/opusfile.c"
			"${OpusfileDir}/src/stream.c"
			)
	else(UseInternalOpus)
		set(IojampLibs ${IojampLibs} opusfile opus)
	endif(UseInternalOpus)
	# Opus needs OGG
	set(NeedOgg ON)
endif(UseCodecOpus)

# OGG (dependency of Opus/Vorbis)
if(NeedOgg)
	if(UseInternalOgg)
		set(IojampIncludeDirs ${IojampIncludeDirs} "${OggDir}/include")
		set(IojampSrcFiles
			${IojampSrcFiles}
			"${OggDir}/src/bitwise.c"
			"${OggDir}/src/framing.c"
			)
	else(UseInternalOgg)
		set(IojampLibs ${IojampLibs} ogg)
	endif(UseInternalOgg)
endif(NeedOgg)

if(UseMumble)
	set(IojampDefines ${IojampDefines} USE_MUMBLE)
	set(IojampSrcFiles ${IojampSrcFiles} "${ClientDir}/libmumblelink.c")
	#TODO: link Mumble?
endif(UseMumble)

if(UseVoip)
	set(IojampDefines ${IojampDefines} USE_VOIP)
	if(UseInternalSpeex)
		set(IojampDefines ${IojampDefines} FLOATING_POINT USE_ALLOCA)
		set(IojampIncludeDirs ${IojampIncludeDirs} "${SpeexDir}/include")
		set(IojampSrcFiles
			${IojampSrcFiles}
			"${SpeexDir}/bits.c"
			"${SpeexDir}/buffer.c"
			"${SpeexDir}/cb_search.c"
			"${SpeexDir}/exc_10_16_table.c"
			"${SpeexDir}/exc_10_32_table.c"
			"${SpeexDir}/exc_20_32_table.c"
			"${SpeexDir}/exc_5_256_table.c"
			"${SpeexDir}/exc_5_64_table.c"
			"${SpeexDir}/exc_8_128_table.c"
			"${SpeexDir}/fftwrap.c"
			"${SpeexDir}/filterbank.c"
			"${SpeexDir}/filters.c"
			"${SpeexDir}/gain_table.c"
			"${SpeexDir}/gain_table_lbr.c"
			"${SpeexDir}/hexc_10_32_table.c"
			"${SpeexDir}/hexc_table.c"
			"${SpeexDir}/high_lsp_tables.c"
			"${SpeexDir}/jitter.c"
			"${SpeexDir}/kiss_fft.c"
			"${SpeexDir}/kiss_fftr.c"
			"${SpeexDir}/lpc.c"
			"${SpeexDir}/lsp.c"
			"${SpeexDir}/lsp_tables_nb.c"
			"${SpeexDir}/ltp.c"
			"${SpeexDir}/mdf.c"
			"${SpeexDir}/modes.c"
			"${SpeexDir}/modes_wb.c"
			"${SpeexDir}/nb_celp.c"
			"${SpeexDir}/preprocess.c"
			"${SpeexDir}/quant_lsp.c"
			"${SpeexDir}/resample.c"
			"${SpeexDir}/sb_celp.c"
			"${SpeexDir}/smallft.c"
			"${SpeexDir}/speex.c"
			"${SpeexDir}/speex_callbacks.c"
			"${SpeexDir}/speex_header.c"
			"${SpeexDir}/stereo.c"
			"${SpeexDir}/vbr.c"
			"${SpeexDir}/vq.c"
			"${SpeexDir}/window.c"
			)
	else(UseInternalSpeex)
		set(IojampLibs ${IojampLibs} speex speexdsp)
	endif(UseInternalSpeex)
endif(UseVoip)

if(UseInternalZlib)
	set(IojampDefines ${IojampDefines} NO_GZIP)
	set(IojampIncludeDirs ${IojampIncludeDirs} ${ZlibDir})
	set(IojampSrcFiles
		${IojampSrcFiles}
		"${ZlibDir}/adler32.c"
		"${ZlibDir}/crc32.c"
		"${ZlibDir}/inffast.c"
		"${ZlibDir}/inflate.c"
		"${ZlibDir}/inftrees.c"
		"${ZlibDir}/zutil.c"
		)
else(UseInternalZlib)
	find_package(ZLIB REQUIRED)
	set(IojampIncludeDirs ${IojampIncludeDirs} ${ZLIB_INCLUDE_DIRS})
	set(IojampLibs ${IojampLibs} ${ZLIB_LIBRARIES})
endif(UseInternalZlib)


if(UseRendererDlopen)
	set(IojampDefines ${IojampDefines} USE_RENDERER_DLOPEN)
else(UseRendererDlopen)
	# if the renderer is not dynamically opened at runtime, statically link Renderer GL1 it into the executable instead
	set(IojampSrcFiles ${IojampSrcFiles} ${RendererGl1SrcFiles})
	set(IojampLibs ${IojampLibs} ${RendererGl1Libs})
	set(IojampDefines ${IojampDefines} ${RendererGl1Defines})
	set(IojampIncludeDirs ${IojampIncludeDirs} ${RendererGl1IncludeDirs})
endif(UseRendererDlopen)

if(WIN32)
	set(IojampLibs
		${IojampLibs}
		ws2_32 #WinSock
		winmm
		psapi
		)
endif(WIN32)

add_executable(${IojampName} ${IojampSrcFiles})
target_link_libraries(${IojampName} ${IojampLibs})
set_target_properties(${IojampName} PROPERTIES COMPILE_DEFINITIONS "${IojampDefines}")
set_target_properties(${IojampName} PROPERTIES INCLUDE_DIRECTORIES "${IojampIncludeDirs}")
