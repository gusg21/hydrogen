//
// Created by xavier.olmstead on 9/7/2024.
//
#include "core/physics/rigidbodycomp.h"

#include "core/assetindex.h"

void h_core::physics::RigidbodyComp::init(h_core::AssetIndex rigidbody) {
    this->rigidbody = rigidbody;
}
