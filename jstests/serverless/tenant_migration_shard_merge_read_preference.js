/**
 * Tests that the shard merge protocol cannot start a migration with readPreference other than
 * primary.
 *
 * @tags: [
 *   incompatible_with_eft,
 *   incompatible_with_macos,
 *   incompatible_with_windows_tls,
 *   requires_shard_merge,
 *   requires_persistence,
 *   serverless,
 *   requires_fcv_71,
 * ]
 */

import {extractUUIDFromObject} from "jstests/libs/uuid_util.js";
import {TenantMigrationTest} from "jstests/replsets/libs/tenant_migration_test.js";

const tenantMigrationTest =
    new TenantMigrationTest({name: jsTestName(), enableRecipientTesting: false});

const donorPrimary = tenantMigrationTest.getDonorPrimary();

const failingMigrationOpts = {
    migrationIdString: extractUUIDFromObject(UUID()),
    readPreference: {mode: "secondary"},
    tenantIds: [ObjectId()]
};
assert.commandFailedWithCode(tenantMigrationTest.startMigration(failingMigrationOpts),
                             ErrorCodes.FailedToSatisfyReadPreference);

const succeessfulMigrationOpts = {
    migrationIdString: extractUUIDFromObject(UUID()),
    readPreference: {mode: "primary"},
    tenantIds: [ObjectId()]
};
assert.commandWorked(tenantMigrationTest.startMigration(succeessfulMigrationOpts));

tenantMigrationTest.stop();
