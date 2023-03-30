#pragma once

#include "common.h"
#include "object_manager.h"

#include <vector>

// --- variants for ACTIONS_1
// --------------------------

enum actions1_e : int {
    run = 0,
    crouch,
};

struct action_one_factory_t : public abstract_object_factory_t {
    object_t* create(tags_t tags, int type) override;
};


// this visitor allows to call non-virtual methods (e.g. these methods need not to be in base class)
// implementation hidden, reference to a base object_visitor_t class
std::unique_ptr<object_visitor_t> create_action_one_nonvirtual_visitor();

// this visitor collects all actions which has a 'visible:true' tag
// this is not as elegant as calling virtual 'test_tag()' on each stored
// object_t pointer, as the same operation must be called for every custom type supported by visitor
struct action_one_visible_objects_collector_t : public object_visitor_t {
    // stores list of 'visible' objects
    std::vector<object_t*> visible_actions;
    void store_if_visible(object_t* o) {
        if (o->test_tag("visible", "true"))
            visible_actions.push_back(o);
    }

    void visit(action_run_t& o) override {
        TRACE() << "action_rut_t visited, checking 'visible' tag\n";
        store_if_visible((object_t*)(&o));
    }

    void visit(action_crouch_t& o) override {
        TRACE() << "action_crouch_t visited, checking 'visible' tag\n";
        store_if_visible((object_t*)(&o));
    }

    // fire not used in "action one" set
    void visit(action_fire_t& o) override {}
    // fire die used in "action one" set
    void visit(action_die_t& o) override {}
};

// --- variants for ACTIONS_2
// --------------------------

enum actions2_e : int {
    fire = 0,
    die,
};

struct action_two_factory_t : public abstract_object_factory_t {
    object_t* create(tags_t tags, int type) override;
};
