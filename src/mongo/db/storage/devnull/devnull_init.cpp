/**
 *    Copyright (C) 2018-present MongoDB, Inc.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the Server Side Public License, version 1,
 *    as published by MongoDB, Inc.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    Server Side Public License for more details.
 *
 *    You should have received a copy of the Server Side Public License
 *    along with this program. If not, see
 *    <http://www.mongodb.com/licensing/server-side-public-license>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the Server Side Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#include <memory>
#include <string>

#include "mongo/base/init.h"  // IWYU pragma: keep
#include "mongo/base/status.h"
#include "mongo/base/string_data.h"
#include "mongo/bson/bsonobj.h"
#include "mongo/db/operation_context.h"
#include "mongo/db/service_context.h"
#include "mongo/db/storage/devnull/devnull_kv_engine.h"
#include "mongo/db/storage/storage_engine.h"
#include "mongo/db/storage/storage_engine_impl.h"
#include "mongo/db/storage/storage_engine_init.h"
#include "mongo/db/storage/storage_engine_lock_file.h"
#include "mongo/db/storage/storage_options.h"

namespace mongo {

namespace {
class DevNullStorageEngineFactory : public StorageEngine::Factory {
public:
    std::unique_ptr<StorageEngine> create(OperationContext* opCtx,
                                          const StorageGlobalParams& params,
                                          const StorageEngineLockFile* lockFile) const override {
        StorageEngineOptions options;
        options.directoryPerDB = params.directoryperdb;
        options.forRepair = params.repair;
        options.forRestore = params.restore;
        options.lockFileCreatedByUncleanShutdown = lockFile && lockFile->createdByUncleanShutdown();
        return std::make_unique<StorageEngineImpl>(
            opCtx, std::make_unique<DevNullKVEngine>(), options);
    }

    StringData getCanonicalName() const override {
        return "devnull";
    }

    Status validateMetadata(const StorageEngineMetadata& metadata,
                            const StorageGlobalParams& params) const override {
        return Status::OK();
    }

    BSONObj createMetadataOptions(const StorageGlobalParams& params) const override {
        return BSONObj();
    }
};
}  // namespace

ServiceContext::ConstructorActionRegisterer registerDevNull(
    "RegisterDevNullEngine", [](ServiceContext* service) {
        registerStorageEngine(service, std::make_unique<DevNullStorageEngineFactory>());
    });
}  // namespace mongo
