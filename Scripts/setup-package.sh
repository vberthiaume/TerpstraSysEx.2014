#!/bin/zsh

USAGE="usage: setup-package.sh <path_ending_in \"Lumatone Editor.app\"> [ --sign-only ]"
if [[ -z $1 || $1 = "--sign-only" ]]; then
    echo $USAGE
    exit 1
fi

if [[ `basename $1` != "Lumatone Editor.app" ]]; then
    echo Invalid application path.
    echo $USAGE
    exit 1
fi

if  [[ ! -d $1 ]]; then
    echo Error: Could not find package directory.
    echo $USAGE
    exit 1
fi

APP_DIR=$1
echo App Dir:$APP_DIR

dir=$(basename `pwd`)
lib_prefix=""
if [[ "$dir" = "Scripts" || "$dir" = "build" ]]; then
    lib_prefix="."
elif [[ "$dir" = "MacOSX" ]]; then
    lib_prefix="../."
fi

LIB_DIR="${lib_prefix}./Libraries/mac/lib"
if [[ ! -d $LIB_DIR ]]; then
    echo Error: libraries not found. This must be called from the project root.
    exit 1
fi

if [[ $2 != "--sign-only" ]]; then 

    LIB_TARGET_PATH=$APP_DIR/Contents/Frameworks
    [[ ! -d $LIB_TARGET_PATH ]] && mkdir -p "${LIB_TARGET_PATH}"

    LIBGPGERR_LINK=libgpg-error.dylib
    LIBGPGERR_OUT=`readlink ${LIB_DIR}/${LIBGPGERR_LINK}`
    LIBGPGERR_BIN=${LIB_DIR}/${LIBGPGERR_OUT}

    ## set r_path relative to app binary
    # echo install_name_tool -add_rpath @executable_path/../Frameworks ${LIBGPGERR_BIN}
    # install_name_tool -add_rpath @executable_path/../Frameworks ${LIBGPGERR_BIN}
    # echo install_name_tool -id @rpath/${LIBGPGERR_LINK} ${LIBGPGERR_BIN}
    # install_name_tool -id @rpath/${LIBGPGERR_LINK} ${LIBGPGERR_BIN}

    # echo cp ${LIBGPGERR_BIN} "${LIB_TARGET_PATH}/${LIBGPGERR_OUT}"
    cp ${LIBGPGERR_BIN} "${LIB_TARGET_PATH}/${LIBGPGERR_LINK}"
    # # ln -s ${LIBGPGERR_BIN} ${LIB_TARGET_PATH}/${LIBGPGERR_LINK} && \
    # # cp -r ${LIBGPGERR_TARGET}/include/* ${COPY_INC_DIR}

    LIBGCRYPT_LINK=libgcrypt.dylib
    LIBGCRYPT_OUT=`readlink ${LIB_DIR}/${LIBGCRYPT_LINK}`
    LIBGCRYPT_BIN=${LIB_DIR}/${LIBGCRYPT_OUT}

    ## set r_path relative to app binary
    # install_name_tool -add_rpath @executable_path/../Frameworks ${LIBGCRYPT_BIN} && echo add rpath successful
    # install_name_tool -change ${LIBGPGERR_BIN} @rpath/${LIBGPGERR_OUT} ${LIBGCRYPT_BIN} && echo change libgpg-error path succesful
    # install_name_tool -id @rpath/${LIBGCRYPT_LINK} ${LIBGCRYPT_BIN} && echo change id successful

    # echo cp ${LIBGCRYPT_BIN} "${LIB_TARGET_PATH}/${LIBGCRYPT_OUT}"
    cp ${LIBGCRYPT_BIN} "${LIB_TARGET_PATH}/${LIBGCRYPT_LINK}"
    # # ln -s ${LIBGCRYPT_BIN} ${COPY_LIB_DIR}/${LIBGCRYPT_LINK} && \
    # # cp -r ${LIBGCRYPT_TARGET}/include/* ${COPY_INC_DIR}

    LIBSSH2_LINK=libssh2.dylib
    LIBSSH2_OUT=`readlink ${LIB_DIR}/${LIBSSH2_LINK}`
    LIBSSH2_BIN=${LIB_DIR}/${LIBSSH2_OUT}

    ## set r_path relative to app binary
    # install_name_tool -add_rpath @executable_path/../Frameworks ${LIBSSH2_BIN}
    # install_name_tool -change ${LIBGCRYPT_BIN} @rpath/${LIBGCRYPT_OUT} ${LIBSSH2_BIN}
    # install_name_tool -change ${LIBGPGERR_BIN} @rpath/${LIBGPGERR_OUT} ${LIBSSH2_BIN}
    # install_name_tool -id @rpath/${LIBSSH2_LINK} ${LIBSSH2_BIN}

    # echo cp ${LIBSSH2_BIN} "${LIB_TARGET_PATH}/${LIBSSH2_OUT}"
    cp ${LIBSSH2_BIN} "${LIB_TARGET_PATH}/${LIBSSH2_LINK}"
    # # ln -s ${LIBSSH2_BIN} ${COPY_LIB_DIR}/${LIBSSH2_LINK} && \
    # # cp -r ${LIBSSH2_TARGET}/include/* ${COPY_INC_DIR}

    echo Fixing LC_RPATH...
    install_name_tool -change @rpath/libssh2.dylib @loader_path/../Frameworks/libssh2.dylib "$APP_DIR/Contents/MacOS/Lumatone Editor"
fi

if [[ ! $LUMATONE_DEV_ID ]]; then
    echo LUMATONE_DEV_ID is not defined.
    [[ $2 == "--sign-only" ]] && exit 1
else
    echo Signing...
    find "$APP_DIR" -type f -exec codesign -f -v -s ${LUMATONE_DEV_ID} {} \;
fi

exit 0
