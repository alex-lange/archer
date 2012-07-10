//
// File: graph_command.h
//

#include "command.h"
#include "g.h"

class get_edges_command : public GraphCommand{
 public:
  get_edges_command();
  void execute( g * graph, vector<string> args);
};


class order_command : public GraphCommand{
 public:
  order_command();
  void execute( g * graph, vector<string> args);
};


class num_tri_command : public GraphCommand{
 public:
  num_tri_command();
  void execute( g * graph, vector<string> args);
};


class max_clique_command : public GraphCommand{
 public:
  max_clique_command();
  void execute( g * graph, vector<string> args);
};


class max_is_command : public GraphCommand{
 public:
  max_is_command();
  void execute( g * graph, vector<string> args);
};


class remove_edge_command : public GraphCommand{
 public:
  remove_edge_command();
  void execute( g * graph, vector<string> args);
};


class add_edge_command : public GraphCommand{
 public:
  add_edge_command();
  void execute( g * graph, vector<string> args);
};

class add_circ_edge_command : public GraphCommand{
 public:
  add_circ_edge_command();
  void execute( g * graph, vector<string> args);
};


class remove_circ_edge_command : public GraphCommand{
 public:
  remove_circ_edge_command();
  void execute( g * graph, vector<string> args);
};


class remove_dist_vs_command : public GraphCommand{
 public:
  remove_dist_vs_command();
  void execute( g * graph, vector<string> args );
};


class remove_rand_vs_command : public GraphCommand{
 public:
  remove_rand_vs_command();
  void execute( g * graph, vector<string> args );
};


class remove_max_is_command : public GraphCommand{
 public:
  remove_max_is_command();
  void execute( g * graph, vector<string> args );
};



class k4_free_proc_command : public GraphCommand{
 public:
  k4_free_proc_command();
  void execute( g * graph, vector<string> args );
};


class make_complement_command : public GraphCommand{
 public:
  make_complement_command();
  void execute( g * graph, vector<string> args);
};


class make_res_circ_command : public GraphCommand{
 public:
  make_res_circ_command();
  void execute( g * graph, vector<string> args);
};


class make_l_circ_command : public GraphCommand{
 public:
  make_l_circ_command();
  void execute( g * graph, vector<string> args);
};


class make_cyc_command : public GraphCommand{
 public:
  make_cyc_command();
  void execute( g * graph, vector<string> args);
};


class make_comp_command : public GraphCommand{
 public:
  make_comp_command();
  void execute( g * graph, vector<string> args);
};


class make_circ_command : public GraphCommand{
 public:
  make_circ_command();
  void execute( g * graph, vector<string> args);
};


class make_galois_circ_command : public GraphCommand{
 public:
  make_galois_circ_command();
  void execute( g * graph, vector<string> args);
};


class make_projective_plane_command : public GraphCommand{
 public:
  make_projective_plane_command();
  void execute( g * graph, vector<string> args);
};


class make_projective_plane_cut_command : public GraphCommand{
 public:
  make_projective_plane_cut_command();
  void execute( g * graph, vector<string> args);
};


class make_embedded_rc_command : public GraphCommand{
 public:
  make_embedded_rc_command();
  void execute( g * graph, vector<string> args);
};


class load_adj_command : public GraphCommand{
 public:
  load_adj_command();
  void execute( g * graph, vector<string> args);
};


class make_joined_command : public GraphCommand{
 public:
  make_joined_command();
  void execute( g * graph, vector<string> args);
  void execute( g * graph, vector<g*> args);
};


class make_connected_command : public GraphCommand{
 public:
  make_connected_command();
  void execute( g * graph, vector<string> args);
  void execute( g * graph, vector<g*> args);
};


class make_avoid_connected_command : public GraphCommand{
 public:
  make_avoid_connected_command();
  void execute( g * graph, vector<string> args);
  void execute( g * graph, vector<g*> args);
};


class make_avoid_rand_connected_command : public GraphCommand{
 public:
  make_avoid_rand_connected_command();
  void execute( g * graph, vector<string> args);
  void execute( g * graph, vector<g*> args);
};


class add_all_noncrit_command : public GraphCommand{
 public:
  add_all_noncrit_command();
  void execute( g * graph, vector<string> args);
};


class add_all_ce_command : public GraphCommand{
 public:
  add_all_ce_command();
  void execute( g * graph, vector<string> args);
};

class add_all_cer_command : public GraphCommand{
 public:
  add_all_cer_command();
  void execute( g * graph, vector<string> args);
};

class remove_k_command : public GraphCommand{
 public:
  remove_k_command();
  void execute( g * graph, vector<string> args);
};

class count_k_command : public GraphCommand{
 public:
  count_k_command();
  void execute( g * graph, vector<string> args);
};


class has_c_command : public GraphCommand{
 public:
  has_c_command();
  void execute( g * graph, vector<string> args);
};


class print_all_command : public GraphCommand{
 public:
  print_all_command();
  void execute( g * graph, vector<string> args);
};


class print_command : public GraphCommand{
 public:
  print_command();
  void execute( g * graph, vector<string> args);
};


class printg6_command : public GraphCommand{
 public:
  printg6_command();
  void execute( g * graph, vector<string> args);
};


class print_sparse_command : public GraphCommand{
 public:
  print_sparse_command();
  void execute( g * graph, vector<string> args);
};

class print_sdpa_command : public GraphCommand{
 public:
  print_sdpa_command();
  void execute( g * graph, vector<string> args);
};


class print_rudy_command : public GraphCommand{
 public:
  print_rudy_command();
  void execute( g * graph, vector<string> args);
};


class print_sat_command : public GraphCommand{
 public:
  print_sat_command();
  void execute( g * graph, vector<string> args);
};


class print_wsat_command : public GraphCommand{
 public:
  print_wsat_command();
  void execute( g * graph, vector<string> args);
};


class print_sat34_command : public GraphCommand{
 public:
  print_sat34_command();
  void execute( g * graph, vector<string> args);
};

class print_satv44_command : public GraphCommand{
 public:
  print_satv44_command();
  void execute( g * graph, vector<string> args);
};

