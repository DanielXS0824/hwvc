/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwFBObject.h"

HwAbsFBObject *HwFBObject::alloc() {
    return new HwFBObject();
}

HwFBObject::HwFBObject() : HwAbsFBObject() {

}

HwFBObject::~HwFBObject() {

}