/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImageLayerModel.h"
#include "AlCropOperateModel.h"
#include "AlAlignCropOperateModel.h"
#include "StringUtils.h"
#include "Logcat.h"

#define TAG "AlImageLayerModel"
#define TAG_LAYER                           "layer"
#define TAG_SCALE                           "scale"
#define TAG_ROTATION                        "rotation"
#define TAG_POSITION                        "position"

#define VAL_PATH                            "path"
#define VAL_ID                              "id"
#define VAL_ALPHA                           "alpha"
#define VAL_RATIONAL_NUM                    "num"
#define VAL_RATIONAL_DEN                    "den"
#define VAL_VEC2_X                          "x"
#define VAL_VEC2_Y                          "y"

AlImageLayerModel *AlImageLayerModel::create(AlIdentityCreator *creator, const std::string path) {
    return create(creator->generate(), path);
}

AlImageLayerModel *AlImageLayerModel::create(int32_t id, const std::string path) {
    return new AlImageLayerModel(id, path);
}

AlImageLayerModel::AlImageLayerModel(int32_t id, const std::string &path)
        : AlAbsElemented(),
          path(path),
          id(id),
          alpha(1.0f),
          rotation(AlRational()),
          position(AlVec2(0.0f, 0.0f)),
          scale(AlVec2(1.0f, 1.0f)) {

}

AlImageLayerModel::AlImageLayerModel(const AlImageLayerModel &o)
        : AlAbsElemented(),
          id(o.id),
          path(o.path),
          alpha(o.alpha),
          rotation(o.rotation),
          position(o.position),
          scale(o.scale) {

}

AlImageLayerModel::~AlImageLayerModel() {
    auto itr = operators.begin();
    while (operators.end() != itr) {
        delete *itr;
        ++itr;
    }
    operators.clear();
}

std::string AlImageLayerModel::getPath() {
    return path;
}

int32_t AlImageLayerModel::getId() {
    return id;
}

void AlImageLayerModel::setAlpha(float alpha) {
    this->alpha = alpha;
}

float AlImageLayerModel::getAlpha() {
    return alpha;
}

void AlImageLayerModel::setScale(float scaleX, float scaleY) {
    scale.x = scaleX;
    scale.y = scaleY;
}

AlVec2 AlImageLayerModel::getScale() {
    return scale;
}

void AlImageLayerModel::setRotation(AlRational &r) {
    this->rotation.num = r.num;
    this->rotation.den = r.den;
}

AlRational AlImageLayerModel::getRotation() {
    return rotation;
}

void AlImageLayerModel::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
}

void AlImageLayerModel::postPosition(float x, float y) {
    position.x += x;
    position.y += y;
}

AlVec2 AlImageLayerModel::getPosition() {
    return position;
}

void AlImageLayerModel::setQuad(AlPointF leftTop,
                                AlPointF leftBottom,
                                AlPointF rightBottom,
                                AlPointF rightTop) {
    quad.setLeftTop(leftTop.x, leftTop.y);
    quad.setLeftBottom(leftBottom.x, leftBottom.y);
    quad.setRightBottom(rightBottom.x, rightBottom.y);
    quad.setRightTop(rightTop.x, rightTop.y);
}

AlQuad &AlImageLayerModel::getQuad() {
    return quad;
}

void AlImageLayerModel::dump() {
    Logcat::i(TAG, "+--------------------------+");
    Logcat::i(TAG, "layer: %d", id);
    Logcat::i(TAG, "alpha: %f", alpha);
    Logcat::i(TAG, "scale: %f, %f", scale.x, scale.y);
    Logcat::i(TAG, "rotat: %f", rotation.toFloat());
    Logcat::i(TAG, "posit: %f, %f", position.x, position.y);
    Logcat::i(TAG, "quad : [%f,%f]-----[%f,%f]",
              quad.leftTop().x, quad.leftTop().y,
              quad.rightTop().x, quad.rightTop().y);
    Logcat::i(TAG, "quad : [%f,%f]-----[%f,%f]",
              quad.leftBottom().x, quad.leftBottom().y,
              quad.rightBottom().x, quad.rightBottom().y);
    Logcat::i(TAG, "+--------------------------+");
}

HwResult AlImageLayerModel::addOperator(AlAbsOperateModel *opt) {
    operators.push_back(opt);
    return Hw::SUCCESS;
}

std::vector<AlAbsOperateModel *> *AlImageLayerModel::getAllOperators() {
    return &operators;
}

bool AlImageLayerModel::removeCropOperator() {
    bool ret = false;
    auto itr = operators.begin();
    while (itr != operators.end()) {
        AlAbsOperateModel *it = *itr;
        if (typeid(AlCropOperateModel) == typeid(*it)) {
            itr = operators.erase(itr);
            ret = true;
            continue;
        }
        ++itr;
    }
    return ret;
}

bool AlImageLayerModel::removeAlignCropOperator() {
    bool ret = false;
    auto itr = operators.begin();
    while (itr != operators.end()) {
        AlAbsOperateModel *it = *itr;
        if (typeid(AlAlignCropOperateModel) == typeid(*it)) {
            itr = operators.erase(itr);
            ret = true;
            continue;
        }
        ++itr;
    }
    return ret;
}

bool AlImageLayerModel::_removeOperator(type_info type) {
    bool ret = false;
    auto itr = operators.begin();
    while (itr != operators.end()) {
        AlAbsOperateModel *it = *itr;
        if (type == typeid(*it)) {
            itr = operators.erase(itr);
            ret = true;
            continue;
        }
        ++itr;
    }
    return ret;
}

HwResult AlImageLayerModel::fromElement(AlElement *element) {
    return Hw::SUCCESS;
}

HwResult AlImageLayerModel::toElement(AlElement **element) {
    AlElement *root = new AlElement(TAG_LAYER);
    root->addAttr(VAL_PATH, path);
    root->addAttr(VAL_ID, StringUtils::valueOf(id));
    root->addAttr(VAL_ALPHA, StringUtils::valueOf(alpha));
    *element = root;

    AlElement *scale = new AlElement(TAG_SCALE);
    scale->addAttr(VAL_VEC2_X, StringUtils::valueOf(this->scale.x));
    scale->addAttr(VAL_VEC2_Y, StringUtils::valueOf(this->scale.y));

    AlElement *rotation = new AlElement(TAG_ROTATION);
    rotation->addAttr(VAL_RATIONAL_NUM, StringUtils::valueOf(this->rotation.num));
    rotation->addAttr(VAL_RATIONAL_DEN, StringUtils::valueOf(this->rotation.den));

    AlElement *pos = new AlElement(TAG_POSITION);
    pos->addAttr(VAL_VEC2_X, StringUtils::valueOf(this->position.x));
    pos->addAttr(VAL_VEC2_Y, StringUtils::valueOf(this->position.y));

    root->addChild(scale);
    root->addChild(rotation);
    root->addChild(pos);

    size_t size = operators.size();
    for (int i = 0; i < size; ++i) {
        AlElement *opt = nullptr;
        operators[i]->toElement(&opt);
        if (opt) {
            root->addChild(opt);
        }
    }
    return Hw::SUCCESS;
}