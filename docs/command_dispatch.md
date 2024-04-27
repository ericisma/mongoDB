# Command Dispatch

Command dispatch refers to the general process by which client requests are
taken from the network, parsed, sanitized, then finally run on databases.

## Service Entry Points

[Service entry points][service_entry_point_h] fulfill the transition from the
transport layer into command implementations. For each incoming connection
from a client (in the form of a [session][session_h] object), a new dedicated
thread is spawned then detached, and is also assigned a new [session workflow]
[session_workflow_h], responsible for maintaining the workflow of a
single client connection during its lifetime. Central to the entry point is the
`handleRequest()` function, which manages the server-side logic of processing
requests and returns a response message indicating the result of the
corresponding request message. This function is currently implemented by several
subclasses of the parent `ServiceEntryPoint` in order to account for the
differences in processing requests between _mongod_ and _mongos_ -- these
distinctions are reflected in the `ServiceEntryPointMongos` and
`ServiceEntryPointMongod` subclasses (see [here][service_entry_point_mongos_h]
and [here][service_entry_point_mongod_h]). One such distinction is the _mongod_
entry point's use of the `ServiceEntryPointCommon::Hooks` interface, which
provides greater flexibility in modifying the entry point's behavior. This
includes waiting on a read of a particular [read concern][read_concern] level to
be completed, as well as determining whether a read concern can indeed by
satisfied given the current state of the server. Similar functionality exists
for [write concerns][write_concern] as well.

## Strategy

One area in which the _mongos_ entry point differs from its _mongod_ counterpart
is in its usage of the [Strategy class][strategy_h]. `Strategy` operates as a
legacy interface for processing client read, write, and command requests; there
is a near 1-to-1 mapping between its constituent functions and request types
(e.g. `writeOp()` for handling write operation requests, `getMore()` for a
getMore request, etc.). These functions comprise the backbone of the _mongos_
entry point's `handleRequest()` -- that is to say, when a valid request is
received, it is sieved and ultimately passed along to the appropriate Strategy
class member function. The significance of using the Strategy class specifically
with the _mongos_ entry point is that it [facilitates query routing to
shards][mongos_router] in _addition_ to running queries against targeted
databases (see [s/transaction_router.h][transaction_router_h] for finer
details).

## Commands

The [Command class][commands_h] serves as a means of cataloging a server command
as well as ascribing various attributes and behaviors to commands via the [type
system][template_method_pattern], that will likely be used during the lifespan
of a particular server. Construction of a Command should only occur during
server startup. When a new Command is constructed, that Command is stored in a
global `CommandRegistry` object for future reference. There are two kinds of
Command subclasses: `BasicCommand` and `TypedCommand`.

A major distinction between the two is in their implementation of the `parse()`
member function. `parse()` takes in a request and returns a handle to a single
invocation of a particular Command (represented by a `CommandInvocation`), that
can then be used to run the Command. The `BasicCommand::parse()` is a naive
implementation that merely forwards incoming requests to the Invocation and
makes sure that the Command does not support document sequences. The
implementation of `TypedCommand::parse()`, on the other hand, varies depending
on the Request type parameter the Command takes in. Since the `TypedCommand`
accepts requests generated by IDL, the parsing function associated with a usable
Request type must allow it to be parsed as an IDL command. In handling requests,
both the _mongos_ and _mongod_ entry points interact with the Command subclasses
through the `CommandHelpers` struct in order to parse requests and ultimately
run them as Commands.

## See Also

For details on transport internals, including ingress networking, see [this document][transport_internals].

[service_entry_point_h]: ../src/mongo/transport/service_entry_point.h
[session_h]: ../src/mongo/transport/session.h
[session_workflow_h]: ../src/mongo/transport/session_workflow.h
[service_entry_point_mongos_h]: ../src/mongo/s/service_entry_point_mongos.h
[service_entry_point_mongod_h]: ../src/mongo/db/service_entry_point_mongod.h
[read_concern]: https://docs.mongodb.com/manual/reference/read-concern/
[write_concern]: https://docs.mongodb.com/manual/reference/write-concern/
[strategy_h]: ../src/mongo/s/commands/strategy.h
[mongos_router]: https://docs.mongodb.com/manual/core/sharded-cluster-query-router/
[transaction_router_h]: ../src/mongo/s/transaction_router.h
[commands_h]: ../src/mongo/db/commands.h
[template_method_pattern]: https://en.wikipedia.org/wiki/Template_method_pattern
[transport_internals]: ../src/mongo/transport/README.md