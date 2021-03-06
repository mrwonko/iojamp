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
option(UseInternalSpeex "Whether to use the included Speex instead of a locally installed one" ON)
option(UseInternalOgg "Whether to use the included Ogg instead of a locally installed one" ON)
option(UseInternalOpus "Whether to use the included Opus instead of a locally installed one" ON)
option(UseInternalZlib "Whether to use the included ZLIB instead of a locally installed one" ON)
option(UseInternalJpeg "Whether to use the included libjpeg instead of a locally installed one" ON)
option(UseInternalPng "Whether to use the included PNG instead of a locally installed one" ON)
option(UseInternalMad "Whether to use the included Mad instead of a locally installed one" ON)
option(UseRendererDlopen "Whether to load the renderer via dlopen" ON)

set(Version 2.0)
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
#set(NullDir "${CodeDir}/null")
set(UIDir "${CodeDir}/ui")
set(Q3UIDir "${CodeDir}/q3_ui")
set(JpegDir "${CodeDir}/jpeg-8c")
set(PngDir "${CodeDir}/png")
set(MadDir "${CodeDir}/mad")
set(SpeexDir "${CodeDir}/libspeex")
set(OggDir "${CodeDir}/libogg-1.3.0")
set(OpusDir "${CodeDir}/opus-1.0.2")
set(OpusfileDir "${CodeDir}/opusfile-0.2")
set(ZlibDir "${CodeDir}/zlib")
set(IcarusDir "${CodeDir}/icarus")
set(Ghoul2Dir "${CodeDir}/ghoul2")
#set(Q3asmDir "${CodeDir}/tools/asm")
#set(LburgDir "${CodeDir}/tools/lcc/lburg")
#set(Q3cppDir "${CodeDir}/tools/lcc/cpp")
#set(Q3lccEtcDir "${CodeDir}/tools/lcc/etc")
#set(Q3lccSrcDir "${CodeDir}/tools/lcc/src")
#set(SdlHeaderDir "${CodeDir}/SDL12")
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
	"${RendererCommonDir}/tr_image_bmp.c"
	"${RendererCommonDir}/tr_image_jpg.c"
	"${RendererCommonDir}/tr_image_pcx.c"
	"${RendererCommonDir}/tr_image_png.c"
	"${RendererCommonDir}/tr_image_tga.c"
	"${RendererCommonDir}/tr_noise.c"
	"${RendererCommonDir}/iqm.h"
	"${RendererCommonDir}/qgl.h"
	"${RendererCommonDir}/tr_common.h"
	"${RendererCommonDir}/tr_public.h"
	"${RendererCommonDir}/tr_types.h"
	)
set(CommonRendererIncludeDirs ${SDL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
set(CommonRendererLibs ${SDL_LIBRARY} ${OPENGL_LIBRARIES})
set(CommonRendererDefines ${CommonDefines})

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
		"${JpegDir}/jconfig.h"
		"${JpegDir}/jdct.h"
		"${JpegDir}/jerror.h"
		"${JpegDir}/jinclude.h"
		"${JpegDir}/jmemsys.h"
		"${JpegDir}/jmorecfg.h"
		"${JpegDir}/jpegint.h"
		"${JpegDir}/jpeglib.h"
		"${JpegDir}/jversion.h"
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
		
		"${PngDir}/png.h"
		"${PngDir}/pngconf.h"
		"${PngDir}/pngdebug.h"
		"${PngDir}/pnginfo.h"
		"${PngDir}/pnglibconf.h"
		"${PngDir}/pngpriv.h"
		"${PngDir}/pngstruct.h"
		)
	set(CommonRendererIncludeDirs ${CommonRendererIncludeDirs} ${PngDir})
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
		
		"${CommonDir}/cm_local.h"
		"${CommonDir}/cm_patch.h"
		"${CommonDir}/cm_polylib.h"
		"${CommonDir}/cm_public.h"
		"${CommonDir}/ioapi.h"
		"${CommonDir}/puff.h"
		"${CommonDir}/q_platform.h"
		"${CommonDir}/q_shared.h"
		"${CommonDir}/qcommon.h"
		"${CommonDir}/qfiles.h"
		"${CommonDir}/surfaceflags.h"
		"${CommonDir}/unzip.h"
		"${CommonDir}/vm_local.h"
		"${CommonDir}/vm_powerpc_asm.h"
		"${CommonDir}/vm_sparc.h"
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
	"${RendererGl1Dir}/tr_font.c"
	
	"${RendererGl1Dir}/tr_local.h"
	
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
	"${CommonDir}/cm_local.h"
	"${CommonDir}/cm_patch.h"
	"${CommonDir}/cm_polylib.h"
	"${CommonDir}/cm_public.h"
	"${CommonDir}/ioapi.h"
	"${CommonDir}/puff.h"
	"${CommonDir}/q_platform.h"
	"${CommonDir}/q_shared.h"
	"${CommonDir}/qcommon.h"
	"${CommonDir}/qfiles.h"
	"${CommonDir}/surfaceflags.h"
	"${CommonDir}/unzip.h"
	"${CommonDir}/vm_local.h"
	"${CommonDir}/vm_powerpc_asm.h"
	"${CommonDir}/vm_sparc.h"
	
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
	"${ClientDir}/cl_curl.h"
	"${ClientDir}/client.h"
	"${ClientDir}/keycodes.h"
	"${ClientDir}/keys.h"
	"${ClientDir}/libmumblelink.h"
	"${ClientDir}/qal.h"
	"${ClientDir}/snd_local.h"
	"${ClientDir}/snd_public.h"
	
	"${CommonDir}/stringed.c"
	
	"${ClientDir}/snd_adpcm.c"
	"${ClientDir}/snd_dma.c"
	"${ClientDir}/snd_mem.c"
	"${ClientDir}/snd_mix.c"
	"${ClientDir}/snd_wavelet.c"
	"${ClientDir}/snd_as.c"
	
	"${ClientDir}/snd_main.c"
	"${ClientDir}/snd_codec.c"
	"${ClientDir}/snd_codec_wav.c"
	"${ClientDir}/snd_codec_ogg.c"
	"${ClientDir}/snd_codec_opus.c"
	"${ClientDir}/snd_codec_mp3.c"
	"${ClientDir}/snd_codec.h"
	
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
	"${ServerDir}/server.h"
	
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
	"${SdlDir}/sdl_icon.h"
	
	#"${AsmDir}/snd_mixa.s"
	#"${AsmDir}/matha.s"
	#"${AsmDir}/snapvector.asm"
	#"${AsmDir}/ftola.asm"
	"${AsmDir}/ftola.c"
	"${AsmDir}/snapvector.c"
	
	"${SysDir}/con_log.c"
	"${SysDir}/sys_main.c"
	"${SysDir}/sys_loadlib.h"
	"${SysDir}/sys_local.h"
	
	"${UIDir}/ui_public.h"
	"${CGameDir}/cg_public.h"
	"${GameDir}/bg_public.h"
	"${GameDir}/bg_weapons.h"
	"${GameDir}/bg_vehicles.h"
	"${GameDir}/anims.h"
	"${GameDir}/g_public.h"
	"${CodeDir}/namespace_begin.h"
	"${CodeDir}/namespace_end.h"
	
	"${IcarusDir}/interpreter.h"
	"${IcarusDir}/Q3_Interface.h"
	"${IcarusDir}/Q3_Registers.h"
	"${Ghoul2Dir}/G2.h"
	"${RendererGl1Dir}/tr_model_ghoul2.c"
	
	"${ClientDir}/fx_load.c"
	"${ClientDir}/fx_local.h"
	"${ClientDir}/fx_main.c"
	"${ClientDir}/fx_primitive.c"
	"${ClientDir}/fx_primitive_camerashake.c"
	"${ClientDir}/fx_primitive_cylinder.c"
	"${ClientDir}/fx_primitive_decal.c"
	"${ClientDir}/fx_primitive_electricity"
	"${ClientDir}/fx_primitive_emitter"
	"${ClientDir}/fx_primitive_flash.c"
	"${ClientDir}/fx_primitive_fxrunner.c"
	"${ClientDir}/fx_primitive_light.c"
	"${ClientDir}/fx_primitive_line.c"
	"${ClientDir}/fx_primitive_orparticle.c"
	"${ClientDir}/fx_primitive_particle.c"
	"${ClientDir}/fx_primitive_sound.c"
	"${ClientDir}/fx_primitive_tail.c"
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
	"${BotlibDir}/aasfile.h"
	"${BotlibDir}/be_aas.h"
	"${BotlibDir}/be_aas_bsp.h"
	"${BotlibDir}/be_aas_cluster.h"
	"${BotlibDir}/be_aas_debug.h"
	"${BotlibDir}/be_aas_def.h"
	"${BotlibDir}/be_aas_entity.h"
	"${BotlibDir}/be_aas_file.h"
	"${BotlibDir}/be_aas_funcs.h"
	"${BotlibDir}/be_aas_main.h"
	"${BotlibDir}/be_aas_move.h"
	"${BotlibDir}/be_aas_optimize.h"
	"${BotlibDir}/be_aas_reach.h"
	"${BotlibDir}/be_aas_route.h"
	"${BotlibDir}/be_aas_routealt.h"
	"${BotlibDir}/be_aas_sample.h"
	"${BotlibDir}/be_ai_char.h"
	"${BotlibDir}/be_ai_chat.h"
	"${BotlibDir}/be_ai_gen.h"
	"${BotlibDir}/be_ai_goal.h"
	"${BotlibDir}/be_ai_move.h"
	"${BotlibDir}/be_ai_weap.h"
	"${BotlibDir}/be_ai_weight.h"
	"${BotlibDir}/be_ea.h"
	"${BotlibDir}/be_interface.h"
	"${BotlibDir}/botlib.h"
	"${BotlibDir}/l_crc.h"
	"${BotlibDir}/l_libvar.h"
	"${BotlibDir}/l_log.h"
	"${BotlibDir}/l_memory.h"
	"${BotlibDir}/l_precomp.h"
	"${BotlibDir}/l_script.h"
	"${BotlibDir}/l_struct.h"
	"${BotlibDir}/l_utils.h"
	)
set_source_files_properties(${IojampBotlibSrcFiles} PROPERTIES COMPILE_DEFINITIONS "BOTLIB")
set(IojampSrcFiles ${IojampSrcFiles} ${IojampBotlibSrcFiles})
set(IojampLibs "")
set(IojampDefines ${CommonDefines}) # NO_VM_COMPILED)
set(IojampIncludeDirs "${CommonDir}")

if(WIN32)
	set(IojampSrcFiles
		${IojampSrcFiles}
		"${SysDir}/con_passive.c"
		"${SysDir}/win_resource.rc"
		"${SysDir}/sys_win32.c"
		
		"${SysDir}/win_resource.h"
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
			
			"${OpusDir}/src/opus_private.h"
			
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
			
			"${OpusDir}/celt/_kiss_fft_guts.h"
			"${OpusDir}/celt/arch.h"
			"${OpusDir}/celt/bands.h"
			"${OpusDir}/celt/celt.h"
			"${OpusDir}/celt/celt_lpc.h"
			"${OpusDir}/celt/cwrs.h"
			"${OpusDir}/celt/ecintrin.h"
			"${OpusDir}/celt/entcode.h"
			"${OpusDir}/celt/entdec.h"
			"${OpusDir}/celt/entenc.h"
			"${OpusDir}/celt/fixed_debug.h"
			"${OpusDir}/celt/fixed_generic.h"
			"${OpusDir}/celt/float_cast.h"
			"${OpusDir}/celt/kiss_fft.h"
			"${OpusDir}/celt/laplace.h"
			"${OpusDir}/celt/mathops.h"
			"${OpusDir}/celt/mdct.h"
			"${OpusDir}/celt/mfrngcod.h"
			"${OpusDir}/celt/modes.h"
			"${OpusDir}/celt/os_support.h"
			"${OpusDir}/celt/pitch.h"
			"${OpusDir}/celt/quant_bands.h"
			"${OpusDir}/celt/rate.h"
			"${OpusDir}/celt/stack_alloc.h"
			"${OpusDir}/celt/static_modes_fixed.h"
			"${OpusDir}/celt/static_modes_float.h"
			"${OpusDir}/celt/vq.h"
			
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
			
			"${OpusDir}/silk/API.h"
			"${OpusDir}/silk/Inlines.h"
			"${OpusDir}/silk/MacroCount.h"
			"${OpusDir}/silk/MacroDebug.h"
			"${OpusDir}/silk/PLC.h"
			"${OpusDir}/silk/SigProc_FIX.h"
			"${OpusDir}/silk/control.h"
			"${OpusDir}/silk/debug.h"
			"${OpusDir}/silk/define.h"
			"${OpusDir}/silk/errors.h"
			"${OpusDir}/silk/macros.h"
			"${OpusDir}/silk/main.h"
			"${OpusDir}/silk/pitch_est_defines.h"
			"${OpusDir}/silk/resampler_private.h"
			"${OpusDir}/silk/resampler_rom.h"
			"${OpusDir}/silk/resampler_structs.h"
			"${OpusDir}/silk/structs.h"
			"${OpusDir}/silk/tables.h"
			"${OpusDir}/silk/tuning_parameters.h"
			"${OpusDir}/silk/typedef.h"
			
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
			
			"${OpusDir}/silk/float/SigProc_FLP.h"
			"${OpusDir}/silk/float/main_FLP.h"
			"${OpusDir}/silk/float/structs_FLP.h"

			"${OpusDir}/include/opus.h"
			"${OpusDir}/include/opus_custom.h"
			"${OpusDir}/include/opus_defines.h"
			"${OpusDir}/include/opus_multistream.h"
			"${OpusDir}/include/opus_types.h"
			
			"${OpusfileDir}/src/http.c"
			"${OpusfileDir}/src/info.c"
			"${OpusfileDir}/src/internal.c"
			"${OpusfileDir}/src/opusfile.c"
			"${OpusfileDir}/src/stream.c"
			
			"${OpusfileDir}/include/opusfile.h"
			"${OpusfileDir}/src/internal.h"
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
			
			"${OggDir}/include/ogg/config_types.h"
			"${OggDir}/include/ogg/ogg.h"
			"${OggDir}/include/ogg/os_types.h"
			)
	else(UseInternalOgg)
		set(IojampLibs ${IojampLibs} ogg)
	endif(UseInternalOgg)
endif(NeedOgg)

# MAD
# search for library, including local one
find_library(MadLib NAMES mad libmad
	HINTS "${LibsDir}/win32" "${LibsDir}/win64" $ENV{MADDIR}
	PATH_SUFFIXES lib lib64
	)
find_path(MadIncludeDir NAMES mad.h
	HINTS "${MadDir}" $ENV{MADDIR}
	PATH_SUFFIXES include
	PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local/include/SDL12
	/usr/local/include/SDL11 # FreeBSD ports
	/usr/include/SDL12
	/usr/include/SDL11
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	)
if( (NOT MadLib) OR (NOT MadIncludeDir) )
	message(FATAL_ERROR "Could not find Mad!")
endif( (NOT MadLib) OR (NOT MadIncludeDir) )
set(IojampIncludeDirs ${IojampIncludeDirs} ${MadIncludeDir})
set(IojampLibs ${IojampLibs} ${MadLib})

if(UseMumble)
	set(IojampDefines ${IojampDefines} USE_MUMBLE)
	set(IojampSrcFiles ${IojampSrcFiles} "${ClientDir}/libmumblelink.c")
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
			
			"${SpeexDir}/_kiss_fft_guts.h"
			"${SpeexDir}/arch.h"
			"${SpeexDir}/cb_search.h"
			"${SpeexDir}/cb_search_arm4.h"
			"${SpeexDir}/cb_search_bfin.h"
			"${SpeexDir}/cb_search_sse.h"
			"${SpeexDir}/config.h"
			"${SpeexDir}/fftwrap.h"
			"${SpeexDir}/filterbank.h"
			"${SpeexDir}/filters.h"
			"${SpeexDir}/filters_arm4.h"
			"${SpeexDir}/filters_bfin.h"
			"${SpeexDir}/filters_sse.h"
			"${SpeexDir}/fixed_arm4.h"
			"${SpeexDir}/fixed_arm5e.h"
			"${SpeexDir}/fixed_bfin.h"
			"${SpeexDir}/fixed_debug.h"
			"${SpeexDir}/fixed_generic.h"
			"${SpeexDir}/kiss_fft.h"
			"${SpeexDir}/kiss_fftr.h"
			"${SpeexDir}/lpc.h"
			"${SpeexDir}/lpc_bfin.h"
			"${SpeexDir}/lsp.h"
			"${SpeexDir}/lsp_bfin.h"
			"${SpeexDir}/ltp.h"
			"${SpeexDir}/ltp_arm4.h"
			"${SpeexDir}/ltp_bfin.h"
			"${SpeexDir}/ltp_sse.h"
			"${SpeexDir}/math_approx.h"
			"${SpeexDir}/misc_bfin.h"
			"${SpeexDir}/modes.h"
			"${SpeexDir}/nb_celp.h"
			"${SpeexDir}/os_support.h"
			"${SpeexDir}/pseudofloat.h"
			"${SpeexDir}/quant_lsp.h"
			"${SpeexDir}/quant_lsp_bfin.h"
			"${SpeexDir}/sb_celp.h"
			"${SpeexDir}/smallft.h"
			"${SpeexDir}/stack_alloc.h"
			"${SpeexDir}/vbr.h"
			"${SpeexDir}/vorbis_psy.h"
			"${SpeexDir}/vq.h"
			"${SpeexDir}/vq_arm4.h"
			"${SpeexDir}/vq_bfin.h"
			"${SpeexDir}/vq_sse.h"
			
			"${SpeexDir}/include/speex/speex.h"
			"${SpeexDir}/include/speex/speex_bits.h"
			"${SpeexDir}/include/speex/speex_buffer.h"
			"${SpeexDir}/include/speex/speex_callbacks.h"
			"${SpeexDir}/include/speex/speex_config_types.h"
			"${SpeexDir}/include/speex/speex_echo.h"
			"${SpeexDir}/include/speex/speex_header.h"
			"${SpeexDir}/include/speex/speex_jitter.h"
			"${SpeexDir}/include/speex/speex_preprocess.h"
			"${SpeexDir}/include/speex/speex_resampler.h"
			"${SpeexDir}/include/speex/speex_stereo.h"
			"${SpeexDir}/include/speex/speex_types.h"
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
		"${ZlibDir}/compress.c"
		"${ZlibDir}/deflate.c"
		"${ZlibDir}/trees.c"
		
		"${ZlibDir}/crc32.h"
		"${ZlibDir}/deflate.h"
		"${ZlibDir}/inffast.h"
		"${ZlibDir}/inffixed.h"
		"${ZlibDir}/inflate.h"
		"${ZlibDir}/inftrees.h"
		"${ZlibDir}/trees.h"
		"${ZlibDir}/zconf.h"
		"${ZlibDir}/zlib.h"
		"${ZlibDir}/zutil.h"
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
