module ApolloError = ApolloClient__Errors_ApolloError;
module Graphql = ApolloClient__Graphql;
module GraphQLError = ApolloClient__Graphql.Error.GraphQLError;
module Observable = ApolloClient__ZenObservable.Observable;
module Types = ApolloClient__Types;
module Utils = ApolloClient__Utils;

module GraphQLRequest = {
  module Js_ = {
    // export interface GraphQLRequest {
    //   query: DocumentNode;
    //   variables?: Record<string, any>;
    //   operationName?: string;
    //   context?: Record<string, any>;
    //   extensions?: Record<string, any>;
    // }
    type t = {
      query: Graphql.documentNode,
      variables: option(Js.Json.t),
      operationName: option(string),
      context: option(Js.Json.t),
      extensions: option(Js.Json.t),
    };
  };

  type t = Js_.t;
};

module Operation = {
  type useMethodFunctionInThisModuleInstead;
  module Js_ = {
    // export interface Operation {
    //     query: DocumentNode;
    //     variables: Record<string, any>;
    //     operationName: string;
    //     extensions: Record<string, any>;
    //     setContext: (context: Record<string, any>) => Record<string, any>;
    //     getContext: () => Record<string, any>;
    // }
    type t = {
      query: Graphql.documentNode,
      variables: Js.Json.t,
      operationName: string,
      extensions: Js.Json.t,
    };

    [@bs.send] external getContext: t => Js.Json.t = "getContext";

    [@bs.send] external setContext: (t, Js.Json.t) => Js.Json.t = "setContext";
  };

  type t = {
    query: Graphql.documentNode,
    variables: Js.Json.t,
    operationName: string,
    extensions: Js.Json.t,
    setContext: Js.Json.t => Js.Json.t,
    getContext: unit => Js.Json.t,
  };

  let fromJs: Js_.t => t =
    js => {
      query: js.query,
      variables: js.variables,
      operationName: js.operationName,
      extensions: js.extensions,
      setContext: context => js->Js_.setContext(context),
      getContext: () => js->Js_.getContext,
    };
};

module FetchResult = {
  module Js_ = {
    // export interface FetchResult<TData = {
    //     [key: string]: any;
    // }, C = Record<string, any>, E = Record<string, any>> extends ExecutionResult {
    //     data?: TData | null;
    //     extensions?: E;
    //     context?: C;
    // }
    type t('jsData) = {
      data: Js.Nullable.t('jsData),
      extensions: option(Js.Json.t), // ACTUAL: Record<string, any>
      context: option(Js.Json.t), // ACTUAL: Record<string, any>
      // ...extends ExecutionResult
      errors: option(array(GraphQLError.t)),
    };
  };

  type t('data) = {
    data: option('data),
    extensions: option(Js.Json.t), // ACTUAL: Record<string, any>
    context: option(Js.Json.t), // ACTUAL: Record<string, any>
    // ...extends ExecutionResult
    error: option(ApolloError.t),
  };

  let fromJs:
    (Js_.t('jsData), ~safeParse: Types.safeParse('data, 'jsData)) =>
    t('data) =
    (js, ~safeParse) => {
      let (data, error) =
        Utils.safeParseAndLiftToCommonResultProps(
          ~jsData=js.data->Js.toOption,
          ~graphQLErrors=?js.errors,
          safeParse,
        );
      {data, error, extensions: js.extensions, context: js.context};
    };

  let fromError: ApolloError.t => t('data) =
    error => {
      data: None,
      extensions: None,
      context: None,
      error: Some(error),
    };
};

module NextLink = {
  module Js_ = {
    // export declare type NextLink = (operation: Operation) => Observable<FetchResult>;
    type t = Operation.Js_.t => Observable.t(FetchResult.Js_.t(Js.Json.t));
  };

  // These are intentionally Js_.t because we can't know what to parse
  type t = Js_.t;
};

module RequestHandler = {
  module Js_ = {
    // export declare type RequestHandler = (operation: Operation, forward: NextLink) => Observable<FetchResult> | null;
    type t =
      (. Operation.Js_.t, NextLink.Js_.t) =>
      Js.Null.t(Observable.t(FetchResult.Js_.t(Js.Json.t)));
  };

  // These are intentionally Js_.t because we can't know what to parse
  type t =
    (Operation.Js_.t, NextLink.Js_.t) =>
    option(Observable.t(FetchResult.Js_.t(Js.Json.t)));

  let toJs: t => Js_.t =
    t => (. operation, forward) => t(operation, forward)->Js.Null.fromOption;
};
