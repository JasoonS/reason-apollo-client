module DataProxy = ApolloClient__Cache_Core_Types.DataProxy;
module Types = ApolloClient__Types;
module Utils = ApolloClient__Utils;

module type Fragment = Types.Fragment;
module type Operation = Types.Operation;
module type OperationNoRequiredVars = Types.OperationNoRequiredVars;

module ApolloCache = {
  module Js_ = {
    // export declare abstract class ApolloCache<TSerialized> implements DataProxy {
    //     abstract read<T, TVariables = any>(query: Cache.ReadOptions<TVariables>): T | null;
    //     abstract write<TResult = any, TVariables = any>(write: Cache.WriteOptions<TResult, TVariables>): Reference | undefined;
    //     abstract diff<T>(query: Cache.DiffOptions): Cache.DiffResult<T>;
    //     abstract watch(watch: Cache.WatchOptions): () => void;
    //     abstract reset(): Promise<void>;
    //     abstract evict(options: Cache.EvictOptions): boolean;
    //     abstract restore(serializedState: TSerialized): ApolloCache<TSerialized>;
    //     abstract extract(optimistic?: boolean): TSerialized;
    //     abstract removeOptimistic(id: string): void;
    //     abstract performTransaction(transaction: Transaction<TSerialized>, optimisticId?: string | null): void;
    //     recordOptimisticTransaction(transaction: Transaction<TSerialized>, optimisticId: string): void;
    //     transformDocument(document: DocumentNode): DocumentNode;
    //     identify(object: StoreObject | Reference): string | undefined;
    //     gc(): string[];
    //     modify(options: Cache.ModifyOptions): boolean;
    //     transformForLink(document: DocumentNode): DocumentNode;
    //     readQuery<QueryType, TVariables = any>(options: DataProxy.Query<TVariables>, optimistic?: boolean): QueryType | null;
    //     private getFragmentDoc;
    //     readFragment<FragmentType, TVariables = any>(options: DataProxy.Fragment<TVariables>, optimistic?: boolean): FragmentType | null;
    //     writeQuery<TData = any, TVariables = any>(options: Cache.WriteQueryOptions<TData, TVariables>): Reference | undefined;
    //     writeFragment<TData = any, TVariables = any>(options: Cache.WriteFragmentOptions<TData, TVariables>): Reference | undefined;
    // }
    type t('tSerialized);

    type reference;

    // readQuery<QueryType, TVariables = any>(options: DataProxy.Query<TVariables>, optimistic?: boolean): QueryType | null;
    [@bs.send]
    external readQuery:
      (
        t('tSerialized),
        ~options: DataProxy.Query.Js_.t('jsVariables),
        ~optimistic: option(bool)
      ) =>
      Js.nullable('jsData) =
      "readQuery";

    // writeFragment<TData = any, TVariables = any>(options: Cache.WriteFragmentOptions<TData, TVariables>): Reference | undefined;
    [@bs.send]
    external writeFragment:
      (
        t('tSerialized),
        ~options: DataProxy.WriteFragmentOptions.Js_.t('jsData, 'jsVariables)
      ) =>
      option(reference) =
      "writeFragment";

    // writeQuery<TData = any, TVariables = any>(options: Cache.WriteQueryOptions<TData, TVariables>): Reference | undefined;
    [@bs.send]
    external writeQuery:
      (
        t('tSerialized),
        ~options: DataProxy.WriteQueryOptions.Js_.t('jsData, 'jsVariables)
      ) =>
      option(reference) =
      "writeQuery";
  };

  type reference = Js_.reference;

  type t('tSerialized) = {
    [@bs.as "reason_readQuery"]
    readQuery:
      'data 'variables 'jsVariables.
      (
        ~query: (module Operation with
                   type t = 'data and
                   type t_variables = 'variables and
                   type Raw.t_variables = 'jsVariables),
        ~id: string=?,
        ~mapJsVariables: 'jsVariables => 'jsVariables=?,
        ~optimistic: bool=?,
        'variables
      ) =>
      option('data),

    [@bs.as "reason_writeFragment"]
    writeFragment:
      'data.
      (
        ~fragment: (module Fragment with type t = 'data),
        ~data: 'data,
        ~broadcast: bool=?,
        ~id: string,
        ~fragmentName: string=?,
        unit
      ) =>
      option(reference),

    [@bs.as "reason_writeQuery"]
    writeQuery:
      'data 'variables 'jsVariables.
      (
        ~query: (module Operation with
                   type t = 'data and
                   type t_variables = 'variables and
                   type Raw.t_variables = 'jsVariables),
        ~broadcast: bool=?,
        ~data: 'data,
        ~id: string=?,
        ~mapJsVariables: 'jsVariables => 'jsVariables=?,
        'variables
      ) =>
      option(reference),

  };

  let preserveJsPropsAndContext: (Js_.t('a), t('a)) => t('a) =
    (js, t) =>
      [%bs.raw
        {|
          function (js, t) {
            return Object.assign(js, t)
          }
        |}
      ](
        js,
        t,
      );

  let fromJs: Js_.t('serialized) => t('serialized) =
    js => {
      let readQuery =
          (
            type data,
            type variables,
            type jsVariables,
            ~query as
              module Operation:
                Operation with
                  type t = data and
                  type t_variables = variables and
                  type Raw.t_variables = jsVariables,
            ~id=?,
            ~mapJsVariables=Utils.identity,
            ~optimistic=?,
            variables,
          ) => {
        js
        ->Js_.readQuery(
            ~options=
              DataProxy.Query.toJs(
                {id, query: Operation.query, variables},
                ~mapJsVariables,
                ~serializeVariables=Operation.serializeVariables,
              ),
            ~optimistic,
          )
        ->Js.toOption
        ->Belt.Option.map(Operation.parse);
      };

      let writeFragment =
          (
            type data,
            ~fragment as module Fragment: Fragment with type t = data,
            ~data: data,
            ~broadcast=?,
            ~id,
            ~fragmentName=?,
            // variables,
            (),
          ) => {
        js->Js_.writeFragment(
          ~options=
            DataProxy.WriteFragmentOptions.toJs(
              {broadcast, data, id, fragment: Fragment.query, fragmentName},
              ~serialize=Fragment.serialize,
            ),
        );
      };

      let writeQuery =
          (
            type data,
            type variables,
            type jsVariables,
            ~query as
              module Operation:
                Operation with
                  type t = data and
                  type t_variables = variables and
                  type Raw.t_variables = jsVariables,
            ~broadcast=?,
            ~data,
            ~id=?,
            ~mapJsVariables=Utils.identity,
            variables,
          ) => {
        js->Js_.writeQuery(
          ~options=
            DataProxy.WriteQueryOptions.toJs(
              {broadcast, data, id, query: Operation.query, variables},
              ~mapJsVariables,
              ~serialize=Operation.serialize,
              ~serializeVariables=Operation.serializeVariables,
            ),
        );
      };

      preserveJsPropsAndContext(js, {readQuery, writeFragment, writeQuery});
    };
};
