#include "entity_actions_implementation.h"

// --- various object_t custom actions derivates ---
// -------------------------------------------------

struct action_run_t : public object_t {
    action_run_t(tags_t t, object_manager_t* m)
    : object_t(t, m)
    {
        tags_.emplace(tags_t::value_type("type", "run"));
    }

    void accept(object_visitor_t* visitor) override {
        TRACE() << "action_run_t: accept\n";
        visitor->visit(*this);
    }

    //intentionaly not virtual, base object_t does not need to be spoiled with every possible action
    void custom_run_action() {
        TRACE() << "action_run : custom action\n";
    }
};

struct action_crouch_t : public object_t {
    action_crouch_t(tags_t t, object_manager_t* m)
    : object_t(t, m)
    {
        tags_.emplace(tags_t::value_type("type", "crouch"));
    }

    void accept(object_visitor_t* visitor) override {
        TRACE() << "action_crouch_t: accept\n";
        visitor->visit(*this);
    }

    //intentionaly not virtual, base object_t does not need to be spoiled with every possible action
    void custom_crouch_action() {
        TRACE() << "action_crouch : custom action\n";
    }
};

struct action_fire_t : public object_t {
    action_fire_t(tags_t t, object_manager_t* m)
    : object_t(t, m)
    {
        tags_.emplace(tags_t::value_type("type", "fire"));
    }

    void accept(object_visitor_t* visitor) override {
        TRACE() << "action_fire_t: accept\n";
        visitor->visit(*this);
    }
};

struct action_die_t : public object_t {
    action_die_t(tags_t t, object_manager_t* m)
    : object_t(t, m)
    {
        tags_.emplace(tags_t::value_type("type", "die"));
    }

    void accept(object_visitor_t* visitor) override {
        TRACE() << "action_die_t: accept\n";
        visitor->visit(*this);
    }

};

// --- ACTIONS_1
object_t* action_one_factory_t::create(tags_t tags, int type) {
    switch (static_cast<actions1_e>(type)) {
        case actions1_e::run:
            return new action_run_t(tags, manager_);
        case actions1_e::crouch:
            return new action_crouch_t(tags, manager_);
        default: ;
    };
    return nullptr;
}

struct actions_one_custom_nonvirtual_action_visitor_t : public object_visitor_t {
    void visit(action_run_t& o) override {
        TRACE() << "action_rut_t visited by action 1 nonvirtual visitor, calling custom_run\n";
        o.custom_run_action();
    }

    void visit(action_crouch_t& o) override {
        TRACE() << "action_crouch_t visited by action 1 nonvirtual visitor, calling custom_run\n";
        o.custom_crouch_action();
    }

    void visit(action_fire_t& o) override {
        TRACE() << "action_fire_t visited by action 1 vistor\n";
    }

    void visit(action_die_t& o) override {
        TRACE() << "action_die_t visited by action 1 vistor\n";
    }
};
std::unique_ptr<object_visitor_t> create_action_one_nonvirtual_visitor() {
    return std::make_unique<actions_one_custom_nonvirtual_action_visitor_t>();
}

// --- ACTIONS_2
object_t* action_two_factory_t::create(tags_t tags, int type) {
    switch (static_cast<actions2_e>(type)) {
        case actions2_e::fire:
            return new action_fire_t(tags, manager_);
        case actions2_e::die:
            return new action_die_t(tags, manager_);
        default: ;
    };
    return nullptr;
}