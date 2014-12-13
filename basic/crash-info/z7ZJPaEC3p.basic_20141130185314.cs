S/W Version Information
Model: Ref.Device-PQ
Tizen-Version: 2.2.1
Build-Number: Tizen_Ref.Device-PQ_20131118.1124
Build-Date: 2013.11.18 11:24:59

Crash Information
Process Name: basic
PID: 12273
Date: 2014-11-30 18:53:14(GMT+0900)
Executable File Path: /opt/apps/z7ZJPaEC3p/bin/basic
This process is multi-thread process
pid=12273 tid=12273
Signal: 11
      (SIGSEGV)
      si_code: -6
      signal sent by tkill (sent by pid 12273, uid 5000)

Register Information
r0   = 0x00000000, r1   = 0x00000001
r2   = 0x00147214, r3   = 0x00000000
r4   = 0x000007de, r5   = 0x0000000b
r6   = 0x00000001, r7   = 0xb3db0d70
r8   = 0xa00009cb, r9   = 0xbe8e2f04
r10  = 0x00000039, fp   = 0xbe8e2ddc
ip   = 0xb3db2da0, sp   = 0xbe8e2cd8
lr   = 0xb36f5edf, pc   = 0xb3712bdc
cpsr = 0x60000030

Memory Information
MemTotal:   797320 KB
MemFree:    195684 KB
Buffers:     55764 KB
Cached:     229568 KB
VmPeak:      84536 KB
VmSize:      84532 KB
VmLck:           0 KB
VmHWM:       21704 KB
VmRSS:       21704 KB
VmData:      11580 KB
VmStk:         136 KB
VmExe:          32 KB
VmLib:       43616 KB
VmPTE:          72 KB
VmSwap:          0 KB

Maps Information
00008000 00010000 r-xp /usr/bin/launchpad_preloading_preinitializing_daemon
00018000 000dc000 rw-p [heap]
000dc000 0055a000 rw-p [heap]
b2499000 b24aa000 r-xp /usr/lib/scim-1.0/1.4.0/IMEngine/socket.so
b24b3000 b24b8000 r-xp /usr/lib/scim-1.0/1.4.0/Config/socket.so
b2512000 b2515000 r-xp /usr/lib/bufmgr/libtbm_exynos4412.so.0.0.0
b251d000 b251e000 r-xp /usr/lib/libdrm_slp.so.1.0.0
b2526000 b2530000 r-xp /usr/lib/evas/modules/engines/software_generic/linux-gnueabi-armv7l-1.7.99/module.so
b255c000 b2570000 r-xp /usr/lib/libnetwork.so.0.0.0
b2579000 b258d000 r-xp /usr/lib/libwifi-direct.so.0.0
b2595000 b259d000 r-xp /usr/lib/libcapi-network-tethering.so.0.1.0
b259e000 b25a7000 r-xp /usr/lib/libcapi-network-connection.so.0.1.3_18
b25af000 b2674000 r-xp /usr/lib/osp/libosp-net.so.1.2.2.0
b2682000 b269f000 r-xp /usr/lib/osp/libosp-image.so.1.2.2.0
b26a8000 b26b1000 r-xp /usr/lib/evas/modules/engines/software_x11/linux-gnueabi-armv7l-1.7.99/module.so
b26ba000 b26c9000 r-xp /opt/usr/apps/z7ZJPaEC3p/bin/basic.exe
b26d2000 b2744000 r-xp /usr/lib/libosp-env-config.so.1.2.2.1
b274c000 b2786000 r-xp /usr/lib/libpulsecommon-0.9.23.so
b278f000 b2793000 r-xp /usr/lib/libmmfsoundcommon.so.0.0.0
b279b000 b27cc000 r-xp /usr/lib/libpulse.so.0.12.4
b27d4000 b2837000 r-xp /usr/lib/libasound.so.2.0.0
b2841000 b2844000 r-xp /usr/lib/libpulse-simple.so.0.0.3
b284c000 b2850000 r-xp /usr/lib/libascenario-0.2.so.0.0.0
b2859000 b2876000 r-xp /usr/lib/libavsysaudio.so.0.0.1
b287e000 b288c000 r-xp /usr/lib/libmmfsound.so.0.1.0
b2894000 b2930000 r-xp /usr/lib/libgstreamer-0.10.so.0.30.0
b293c000 b297d000 r-xp /usr/lib/libgstbase-0.10.so.0.30.0
b2986000 b298f000 r-xp /usr/lib/libgstapp-0.10.so.0.25.0
b2997000 b29a4000 r-xp /usr/lib/libgstinterfaces-0.10.so.0.25.0
b29ad000 b29b3000 r-xp /usr/lib/libUMP.so
b29bb000 b29be000 r-xp /usr/lib/libmm_ta.so.0.0.0
b29c6000 b29d5000 r-xp /usr/lib/libICE.so.6.3.0
b29df000 b29e4000 r-xp /usr/lib/libSM.so.6.0.1
b29ec000 b29ed000 r-xp /usr/lib/libmmfkeysound.so.0.0.0
b29f5000 b29fd000 r-xp /usr/lib/libmmfcommon.so.0.0.0
b2a05000 b2a0d000 r-xp /usr/lib/libaudio-session-mgr.so.0.0.0
b2a18000 b2a1b000 r-xp /usr/lib/libmmfsession.so.0.0.0
b2a23000 b2a67000 r-xp /usr/lib/libmmfplayer.so.0.0.0
b2a70000 b2a83000 r-xp /usr/lib/libEGL_platform.so
b2a8c000 b2b63000 r-xp /usr/lib/libMali.so
b2b6e000 b2b74000 r-xp /usr/lib/libxcb-render.so.0.0.0
b2b7c000 b2b7d000 r-xp /usr/lib/libxcb-shm.so.0.0.0
b2b86000 b2bc4000 r-xp /usr/lib/libGLESv2.so.2.0
b2bcc000 b2c17000 r-xp /usr/lib/libtiff.so.5.1.0
b2c22000 b2c4b000 r-xp /usr/lib/libturbojpeg.so
b2c64000 b2c6a000 r-xp /usr/lib/libmmutil_imgp.so.0.0.0
b2c72000 b2c78000 r-xp /usr/lib/libgif.so.4.1.6
b2c80000 b2ca2000 r-xp /usr/lib/libavutil.so.51.73.101
b2cb1000 b2cdf000 r-xp /usr/lib/libswscale.so.2.1.101
b2ce8000 b2fdf000 r-xp /usr/lib/libavcodec.so.54.59.100
b3306000 b331f000 r-xp /usr/lib/libpng12.so.0.50.0
b3328000 b332e000 r-xp /usr/lib/libfeedback.so.0.1.4
b3336000 b3342000 r-xp /usr/lib/libtts.so
b334a000 b3361000 r-xp /usr/lib/libEGL.so.1.4
b336a000 b3421000 r-xp /usr/lib/libcairo.so.2.11200.12
b342b000 b3445000 r-xp /usr/lib/osp/libosp-image-core.so.1.2.2.0
b344e000 b3d49000 r-xp /usr/lib/osp/libosp-uifw.so.1.2.2.1
b3dbc000 b3dc1000 r-xp /usr/lib/libslp_devman_plugin.so
b3dca000 b3dcd000 r-xp /usr/lib/libsyspopup_caller.so.0.1.0
b3dd5000 b3dd9000 r-xp /usr/lib/libsysman.so.0.2.0
b3de1000 b3df2000 r-xp /usr/lib/libsecurity-server-commons.so.1.0.0
b3dfb000 b3dfd000 r-xp /usr/lib/libsystemd-daemon.so.0.0.1
b3e05000 b3e07000 r-xp /usr/lib/libdeviced.so.0.1.0
b3e0f000 b3e25000 r-xp /usr/lib/libpkgmgr_parser.so.0.1.0
b3e2d000 b3e2f000 r-xp /usr/lib/libpkgmgr_installer_status_broadcast_server.so.0.1.0
b3e37000 b3e3a000 r-xp /usr/lib/libpkgmgr_installer_client.so.0.1.0
b3e42000 b3e45000 r-xp /usr/lib/libdevice-node.so.0.1
b3e4d000 b3e51000 r-xp /usr/lib/libheynoti.so.0.0.2
b3e59000 b3e9e000 r-xp /usr/lib/libsoup-2.4.so.1.5.0
b3ea7000 b3ebc000 r-xp /usr/lib/libsecurity-server-client.so.1.0.1
b3ec5000 b3ec9000 r-xp /usr/lib/libcapi-system-info.so.0.2.0
b3ed1000 b3ed6000 r-xp /usr/lib/libcapi-system-system-settings.so.0.0.2
b3ede000 b3edf000 r-xp /usr/lib/libcapi-system-power.so.0.1.1
b3ee8000 b3eeb000 r-xp /usr/lib/libcapi-system-device.so.0.1.0
b3ef3000 b3ef6000 r-xp /usr/lib/libcapi-system-runtime-info.so.0.0.3
b3eff000 b3f02000 r-xp /usr/lib/libcapi-network-serial.so.0.0.8
b3f0a000 b3f0b000 r-xp /usr/lib/libcapi-content-mime-type.so.0.0.2
b3f13000 b3f21000 r-xp /usr/lib/libcapi-appfw-application.so.0.1.0
b3f2a000 b3f4c000 r-xp /usr/lib/libSLP-tapi.so.0.0.0
b3f54000 b3f57000 r-xp /usr/lib/libuuid.so.1.3.0
b3f60000 b3f7e000 r-xp /usr/lib/libpkgmgr-info.so.0.0.17
b3f86000 b3f9d000 r-xp /usr/lib/libpkgmgr-client.so.0.1.68
b3fa6000 b3fa7000 r-xp /usr/lib/libpmapi.so.1.2
b3faf000 b3fb7000 r-xp /usr/lib/libminizip.so.1.0.0
b3fbf000 b3fca000 r-xp /usr/lib/libmessage-port.so.1.2.2.1
b3fd2000 b40aa000 r-xp /usr/lib/libxml2.so.2.7.8
b40b7000 b40d5000 r-xp /usr/lib/libpcre.so.0.0.1
b40dd000 b40e0000 r-xp /usr/lib/libiniparser.so.0
b40e9000 b40ed000 r-xp /usr/lib/libhaptic.so.0.1
b40f5000 b4100000 r-xp /usr/lib/libcryptsvc.so.0.0.1
b410d000 b4112000 r-xp /usr/lib/libdevman.so.0.1
b411b000 b411f000 r-xp /usr/lib/libchromium.so.1.0
b4127000 b412d000 r-xp /usr/lib/libappsvc.so.0.1.0
b4135000 b4136000 r-xp /usr/lib/osp/libappinfo.so.1.2.2.1
b4146000 b4148000 r-xp /opt/usr/apps/z7ZJPaEC3p/bin/basic
b4150000 b4156000 r-xp /usr/lib/libalarm.so.0.0.0
b415f000 b4171000 r-xp /usr/lib/libprivacy-manager-client.so.0.0.5
b4179000 b4478000 r-xp /usr/lib/osp/libosp-appfw.so.1.2.2.1
b449f000 b44a9000 r-xp /lib/libnss_files-2.13.so
b44b2000 b44bb000 r-xp /lib/libnss_nis-2.13.so
b44c4000 b44d5000 r-xp /lib/libnsl-2.13.so
b44e0000 b44e6000 r-xp /lib/libnss_compat-2.13.so
b44ef000 b44f8000 r-xp /usr/lib/libcapi-security-privilege-manager.so.0.0.3
b4820000 b4831000 r-xp /usr/lib/libcom-core.so.0.0.1
b4839000 b483b000 r-xp /usr/lib/libdri2.so.0.0.0
b4843000 b484b000 r-xp /usr/lib/libdrm.so.2.4.0
b4853000 b4857000 r-xp /usr/lib/libtbm.so.1.0.0
b485f000 b4862000 r-xp /usr/lib/libXv.so.1.0.0
b486a000 b4935000 r-xp /usr/lib/libscim-1.0.so.8.2.3
b494b000 b495b000 r-xp /usr/lib/libnotification.so.0.1.0
b4963000 b4987000 r-xp /usr/lib/ecore/immodules/libisf-imf-module.so
b4990000 b49a0000 r-xp /lib/libresolv-2.13.so
b49a4000 b49a6000 r-xp /usr/lib/libgmodule-2.0.so.0.3200.3
b49ae000 b4b01000 r-xp /usr/lib/libcrypto.so.1.0.0
b4b1f000 b4b6b000 r-xp /usr/lib/libssl.so.1.0.0
b4b77000 b4ba3000 r-xp /usr/lib/libidn.so.11.5.44
b4bac000 b4bb6000 r-xp /usr/lib/libcares.so.2.0.0
b4bbe000 b4bd5000 r-xp /lib/libexpat.so.1.5.2
b4bdf000 b4c03000 r-xp /usr/lib/libicule.so.48.1
b4c0c000 b4c14000 r-xp /usr/lib/libsf_common.so
b4c1c000 b4cb7000 r-xp /usr/lib/libstdc++.so.6.0.14
b4cca000 b4da7000 r-xp /usr/lib/libgio-2.0.so.0.3200.3
b4db2000 b4dd7000 r-xp /usr/lib/libexif.so.12.3.3
b4deb000 b4df5000 r-xp /usr/lib/libethumb.so.1.7.99
b4dfd000 b4e41000 r-xp /usr/lib/libsndfile.so.1.0.25
b4e4f000 b4e51000 r-xp /usr/lib/libctxdata.so.0.0.0
b4e59000 b4e67000 r-xp /usr/lib/libremix.so.0.0.0
b4e6f000 b4e70000 r-xp /usr/lib/libecore_imf_evas.so.1.7.99
b4e78000 b4e91000 r-xp /usr/lib/liblua-5.1.so
b4e9a000 b4ea1000 r-xp /usr/lib/libembryo.so.1.7.99
b4eaa000 b4ead000 r-xp /usr/lib/libecore_input_evas.so.1.7.99
b4eb5000 b4ef2000 r-xp /usr/lib/libcurl.so.4.3.0
b4efc000 b4f00000 r-xp /usr/lib/libecore_ipc.so.1.7.99
b4f09000 b4f73000 r-xp /usr/lib/libpixman-1.so.0.28.2
b4f80000 b4fa4000 r-xp /usr/lib/libfontconfig.so.1.5.0
b4fad000 b5009000 r-xp /usr/lib/libharfbuzz.so.0.907.0
b501b000 b502f000 r-xp /usr/lib/libfribidi.so.0.3.1
b5037000 b508c000 r-xp /usr/lib/libfreetype.so.6.8.1
b5097000 b50bb000 r-xp /usr/lib/libjpeg.so.8.0.2
b50d3000 b50ea000 r-xp /lib/libz.so.1.2.5
b50f2000 b50ff000 r-xp /usr/lib/libsensor.so.1.1.0
b510a000 b510c000 r-xp /usr/lib/libapp-checker.so.0.1.0
b5114000 b511a000 r-xp /usr/lib/libxdgmime.so.1.1.0
b6231000 b6319000 r-xp /usr/lib/libicuuc.so.48.1
b6326000 b6446000 r-xp /usr/lib/libicui18n.so.48.1
b6454000 b6457000 r-xp /usr/lib/libSLP-db-util.so.0.1.0
b645f000 b6468000 r-xp /usr/lib/libvconf.so.0.2.45
b6470000 b647e000 r-xp /usr/lib/libail.so.0.1.0
b6486000 b649e000 r-xp /usr/lib/libdbus-glib-1.so.2.2.2
b649f000 b64a4000 r-xp /usr/lib/libffi.so.5.0.10
b64ac000 b64ad000 r-xp /usr/lib/libgthread-2.0.so.0.3200.3
b64b5000 b64bf000 r-xp /usr/lib/libXext.so.6.4.0
b64c8000 b64cb000 r-xp /usr/lib/libXtst.so.6.1.0
b64d3000 b64d9000 r-xp /usr/lib/libXrender.so.1.3.0
b64e1000 b64e7000 r-xp /usr/lib/libXrandr.so.2.2.0
b64ef000 b64f0000 r-xp /usr/lib/libXinerama.so.1.0.0
b64f9000 b6502000 r-xp /usr/lib/libXi.so.6.1.0
b650a000 b650d000 r-xp /usr/lib/libXfixes.so.3.1.0
b6515000 b6517000 r-xp /usr/lib/libXgesture.so.7.0.0
b651f000 b6521000 r-xp /usr/lib/libXcomposite.so.1.0.0
b6529000 b652a000 r-xp /usr/lib/libXdamage.so.1.1.0
b6533000 b653a000 r-xp /usr/lib/libXcursor.so.1.0.2
b6542000 b654a000 r-xp /usr/lib/libemotion.so.1.7.99
b6552000 b656d000 r-xp /usr/lib/libecore_con.so.1.7.99
b6576000 b657b000 r-xp /usr/lib/libecore_imf.so.1.7.99
b6584000 b658c000 r-xp /usr/lib/libethumb_client.so.1.7.99
b6594000 b6596000 r-xp /usr/lib/libefreet_trash.so.1.7.99
b659e000 b65a2000 r-xp /usr/lib/libefreet_mime.so.1.7.99
b65ab000 b65c1000 r-xp /usr/lib/libefreet.so.1.7.99
b65cb000 b65d4000 r-xp /usr/lib/libedbus.so.1.7.99
b65dc000 b65e1000 r-xp /usr/lib/libecore_fb.so.1.7.99
b65ea000 b6646000 r-xp /usr/lib/libedje.so.1.7.99
b6650000 b6667000 r-xp /usr/lib/libecore_input.so.1.7.99
b6682000 b6687000 r-xp /usr/lib/libecore_file.so.1.7.99
b668f000 b66ac000 r-xp /usr/lib/libecore_evas.so.1.7.99
b66b5000 b66f4000 r-xp /usr/lib/libeina.so.1.7.99
b66fd000 b67ac000 r-xp /usr/lib/libevas.so.1.7.99
b67ce000 b67e1000 r-xp /usr/lib/libeet.so.1.7.99
b67ea000 b6854000 r-xp /lib/libm-2.13.so
b6860000 b6867000 r-xp /usr/lib/libutilX.so.1.1.0
b686f000 b6874000 r-xp /usr/lib/libappcore-common.so.1.1
b687c000 b6887000 r-xp /usr/lib/libaul.so.0.1.0
b6890000 b68c4000 r-xp /usr/lib/libgobject-2.0.so.0.3200.3
b68cd000 b68fd000 r-xp /usr/lib/libecore_x.so.1.7.99
b6906000 b691b000 r-xp /usr/lib/libecore.so.1.7.99
b6932000 b6a52000 r-xp /usr/lib/libelementary.so.1.7.99
b6a65000 b6a68000 r-xp /lib/libattr.so.1.1.0
b6a70000 b6a72000 r-xp /usr/lib/libXau.so.6.0.0
b6a7a000 b6a80000 r-xp /lib/librt-2.13.so
b6a89000 b6a91000 r-xp /lib/libcrypt-2.13.so
b6ac1000 b6ac4000 r-xp /lib/libcap.so.2.21
b6acc000 b6ace000 r-xp /usr/lib/libiri.so
b6ad6000 b6aeb000 r-xp /usr/lib/libxcb.so.1.1.0
b6af3000 b6afe000 r-xp /lib/libunwind.so.8.0.1
b6b2c000 b6c49000 r-xp /lib/libc-2.13.so
b6c57000 b6c60000 r-xp /lib/libgcc_s-4.5.3.so.1
b6c68000 b6c6b000 r-xp /usr/lib/libsmack.so.1.0.0
b6c73000 b6c9f000 r-xp /usr/lib/libdbus-1.so.3.7.2
b6ca8000 b6cac000 r-xp /usr/lib/libbundle.so.0.1.22
b6cb4000 b6cb6000 r-xp /lib/libdl-2.13.so
b6cbf000 b6d99000 r-xp /usr/lib/libglib-2.0.so.0.3200.3
b6da2000 b6e0c000 r-xp /usr/lib/libsqlite3.so.0.8.6
b6e16000 b6e23000 r-xp /usr/lib/libprivilege-control.so.0.0.2
b6e2c000 b6f12000 r-xp /usr/lib/libX11.so.6.3.0
b6f1d000 b6f31000 r-xp /lib/libpthread-2.13.so
b6f41000 b6f45000 r-xp /usr/lib/libappcore-efl.so.1.1
b6f4e000 b6f4f000 r-xp /usr/lib/libdlog.so.0.0.0
b6f57000 b6f5b000 r-xp /usr/lib/libsys-assert.so
b6f63000 b6f80000 r-xp /lib/ld-2.13.so
be8c4000 be8e5000 rwxp [stack]
End of Maps Information

Callstack Information (PID:12273)
Call Stack Count: 34
 0: Tizen::Ui::_ControlImpl::GetFullScreenSizeInfo() + 0x53 (0xb3712bdc) [/usr/lib/osp/libosp-uifw.so] + 0x2c4bdc
 1: Tizen::Ui::Control::SetShowState(bool) + 0xa (0xb36f5edf) [/usr/lib/osp/libosp-uifw.so] + 0x2a7edf
 2: MainForm::OnInitializing() + 0x678 (0xb26c44b0) [/opt/apps/z7ZJPaEC3p/bin/basic.exe] + 0xa4b0
 3: Tizen::Ui::_ControlImpl::OnAttachedToMainTree() + 0x22 (0xb37121a3) [/usr/lib/osp/libosp-uifw.so] + 0x2c41a3
 4: Tizen::Ui::Controls::_FormImpl::OnAttachedToMainTree() + 0x2e (0xb38ecbc3) [/usr/lib/osp/libosp-uifw.so] + 0x49ebc3
 5: Tizen::Ui::_Control::CallOnAttachedToMainTree(Tizen::Ui::_Control&) + 0xb2 (0xb36f90af) [/usr/lib/osp/libosp-uifw.so] + 0x2ab0af
 6: Tizen::Ui::_Control::EndAttaching(Tizen::Ui::_Control&) + 0xfc (0xb36fc969) [/usr/lib/osp/libosp-uifw.so] + 0x2ae969
 7: Tizen::Ui::_Control::AttachChild(Tizen::Ui::_Control&) + 0x72 (0xb36ff4ef) [/usr/lib/osp/libosp-uifw.so] + 0x2b14ef
 8: Tizen::Ui::_ContainerImpl::AddChild(Tizen::Ui::_ControlImpl*, bool) + 0xac (0xb371f051) [/usr/lib/osp/libosp-uifw.so] + 0x2d1051
 9: Tizen::Ui::Container::AddControl(Tizen::Ui::Control*) + 0x26 (0xb36f37b7) [/usr/lib/osp/libosp-uifw.so] + 0x2a57b7
10: BasicAppFrame::OnInitializing() + 0x88 (0xb26c0880) [/opt/apps/z7ZJPaEC3p/bin/basic.exe] + 0x6880
11: Tizen::Ui::_ControlImpl::OnAttachedToMainTree() + 0x22 (0xb37121a3) [/usr/lib/osp/libosp-uifw.so] + 0x2c41a3
12: Tizen::Ui::Controls::_FrameImpl::OnAttachedToMainTree() + 0xa (0xb38f041b) [/usr/lib/osp/libosp-uifw.so] + 0x4a241b
13: Tizen::Ui::_ControlManager::CallOnAttachedToMainTree(Tizen::Ui::_Control&) + 0x76 (0xb3722a9b) [/usr/lib/osp/libosp-uifw.so] + 0x2d4a9b
14: Tizen::Ui::_ControlManager::ActivateWindow(Tizen::Ui::_Window&, bool) + 0xc6 (0xb37237ef) [/usr/lib/osp/libosp-uifw.so] + 0x2d57ef
15: Tizen::Ui::_ControlManager::OpenWindow(Tizen::Ui::_Window&, bool) + 0x2e (0xb3723a07) [/usr/lib/osp/libosp-uifw.so] + 0x2d5a07
16: Tizen::Ui::_Window::Open(bool) + 0x3c (0xb3710505) [/usr/lib/osp/libosp-uifw.so] + 0x2c2505
17: Tizen::Ui::_WindowImpl::Open(bool) + 0x12 (0xb3720a57) [/usr/lib/osp/libosp-uifw.so] + 0x2d2a57
18: Tizen::App::_UiAppImpl::AddFrame(Tizen::Ui::Controls::Frame const&) + 0x40 (0xb3af9db1) [/usr/lib/osp/libosp-uifw.so] + 0x6abdb1
19: Tizen::App::UiApp::AddFrame(Tizen::Ui::Controls::Frame const&) + 0x14 (0xb3af904d) [/usr/lib/osp/libosp-uifw.so] + 0x6ab04d
20: BasicApp::OnAppInitializing(Tizen::App::AppRegistry&) + 0x1cc (0xb26bfe48) [/opt/apps/z7ZJPaEC3p/bin/basic.exe] + 0x5e48
21: Tizen::App::_UiAppImpl::OnAppInitializing() + 0x56 (0xb3af9ecb) [/usr/lib/osp/libosp-uifw.so] + 0x6abecb
22: Tizen::App::_AppImpl::OnCreate(void*) + 0x82 (0xb4268bf7) [/usr/lib/osp/libosp-appfw.so] + 0xefbf7
23: app_appcore_create + 0xc2 (0xb3f1af2b) [/usr/lib/libcapi-appfw-application.so.0] + 0x7f2b
24: appcore_efl_main + 0x228 (0xb6f43729) [/usr/lib/libappcore-efl.so.1] + 0x2729
25: app_efl_main + 0xc6 (0xb3f1b07f) [/usr/lib/libcapi-appfw-application.so.0] + 0x807f
26: Tizen::App::_AppImpl::Execute(Tizen::App::_IAppImpl*) + 0xe6 (0xb42668b3) [/usr/lib/osp/libosp-appfw.so] + 0xed8b3
27: Tizen::App::UiApp::Execute(Tizen::App::UiApp* (*)(), Tizen::Base::Collection::IList const*) + 0x64 (0xb3af9231) [/usr/lib/osp/libosp-uifw.so] + 0x6ab231
28: OspMain + 0x118 (0xb26c0514) [/opt/apps/z7ZJPaEC3p/bin/basic.exe] + 0x6514
29: main + 0x194 (0xb4146f21) [/opt/apps/z7ZJPaEC3p/bin/basic] + 0xf21
30: __launchpad_main_loop + 0xe54 (0xb259) [/usr/bin/launchpad_preloading_preinitializing_daemon] + 0xb259
31: main + 0x486 (0xbcb3) [/usr/bin/launchpad_preloading_preinitializing_daemon] + 0xbcb3
32: __libc_start_main + 0x114 (0xb6b416e8) [/lib/libc.so.6] + 0x156e8
33: (0xa110) [/usr/bin/launchpad_preloading_preinitializing_daemon] + 0xa110
End of Call Stack

Package Information
Package Name: z7ZJPaEC3p.basic
Package ID : z7ZJPaEC3p
Version: 2.0.0
Package Type: tpk
App Name: basic
App ID: z7ZJPaEC3p.basic
Type: Application
Categories: (NULL)
