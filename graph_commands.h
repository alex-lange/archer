//
// File: graph_command.h
//

#include "command.h"
#include "g.h"

class remove_edge_command : public GraphCommand{
 public:
  remove_edge_command();

 public:
  void execute( g * graph, vector<string> args);
};


class add_edge_command : public GraphCommand{
 public:
  add_edge_command();

 public:
  void execute( g * graph, vector<string> args);
};

class add_circ_edge_command : public GraphCommand{
 public:
  add_circ_edge_command();

 public:
  void execute( g * graph, vector<string> args);
};

class remove_circ_edge_command : public GraphCommand{
 public:
  remove_circ_edge_command();

 public:
  void execute( g * graph, vector<string> args);
};

class remove_rand_vs_command : public GraphCommand{
 public:
  remove_rand_vs_command();

 public:
  void execute( g * graph, vector<string> args );
};


class make_res_circ_command : public GraphCommand{
 public:
  make_res_circ_command();

 public:
  void execute( g * graph, vector<string> args);
};


class make_circ_command : public GraphCommand{
 public:
  make_circ_command();

 public:
  void execute( g * graph, vector<string> args);
};


class make_galois_circ_command : public GraphCommand{
 public:
  make_galois_circ_command();

 public:
  void execute( g * graph, vector<string> args);
};


class make_embedded_rc_command : public GraphCommand{
 public:
  make_embedded_rc_command();

 public:
  void execute( g * graph, vector<string> args);
};


class make_joined_command : public GraphCommand{
 public:
  make_joined_command();

 public:
  void execute( g * graph, vector<string> args);
  void execute( g * graph, vector<g*> args);
};


class add_all_noncrit_command : public GraphCommand{
 public:
  add_all_noncrit_command();

 public:
  void execute( g * graph, vector<string> args);
};


class remove_k_command : public GraphCommand{
 public:
  remove_k_command();

 public:
  void execute( g * graph, vector<string> args);
};

class count_k_command : public GraphCommand{
 public:
  count_k_command();

 public:
  void execute( g * graph, vector<string> args);
};


class print_command : public GraphCommand{
 public:
  print_command();

 public:
  void execute( g * graph, vector<string> args);
};

class print_sparse_command : public GraphCommand{
 public:
  print_sparse_command();

 public:
  void execute( g * graph, vector<string> args);
};

class print_sdpa_command : public GraphCommand{
 public:
  print_sdpa_command();

 public:
  void execute( g * graph, vector<string> args);
};


class print_rudy_command : public GraphCommand{
 public:
  print_rudy_command();

 public:
  void execute( g * graph, vector<string> args);
};


class print_sat_command : public GraphCommand{
 public:
  print_sat_command();

 public:
  void execute( g * graph, vector<string> args);
};
