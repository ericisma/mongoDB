/**
 * indexed_insert_1char_noindex.js
 *
 * Executes the indexed_insert_1char.js workload after dropping its index.
 */
import {extendWorkload} from "jstests/concurrency/fsm_libs/extend_workload.js";
import {indexedNoindex} from "jstests/concurrency/fsm_workload_modifiers/indexed_noindex.js";
import {$config as $baseConfig} from "jstests/concurrency/fsm_workloads/indexed_insert_1char.js";

export const $config = extendWorkload($baseConfig, indexedNoindex);
