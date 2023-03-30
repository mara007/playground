#pragma once

#include "common.h"

#include <string>
#include <vector>
#include <mutex>
#include <memory>
#include <functional>

// fwd
class object_manager_t;
class object_t;
// fwd of 'custom actions' derivates from object_t
class action_run_t;
class action_crouch_t;
class action_fire_t;
class action_die_t;

// implements arbitrary action uppon object_t and its inherited classes
// TODO DIRTY: dependency on custom object_t derivates
struct object_visitor_t {
    virtual void visit(action_run_t& obj) = 0;
    virtual void visit(action_crouch_t& obj) = 0;
    virtual void visit(action_fire_t& obj) = 0;
    virtual void visit(action_die_t& obj) = 0;
};


// arbitrary object, tags will be used instead of virtual "CanBeVisible"
class object_t {
    friend class object_manager_t;
    protected:

    tags_t tags_;
    object_manager_t* manager_;

    public:
    object_t(tags_t tags, object_manager_t* man);

    bool test_tag(const std::string& name, const std::string& value);
    tags_t& get_tags() { return tags_; };
    const tags_t& get_tags() const { return tags_; };
    virtual void accept(object_visitor_t* visitor) {};

    //deletes itself from manager
    virtual ~object_t();
};

static std::ostream& operator<<(std::ostream& ostr, const object_t& o) {
    ostr << o.get_tags();
    return ostr;
}

// object factory inteface is used by object_manager_t to instantiate
// object of s desired type
// TODO DIRTY: desired type will be casted to int and from int
struct abstract_object_factory_t {
    object_manager_t* manager_ = nullptr;
    void set_manager(object_manager_t* m) { manager_ = m; }

    // returns a new instance allocated by 'new', so it can be 'delete'd
    virtual object_t* create(tags_t tags, int type) = 0;
    virtual ~abstract_object_factory_t(){}
};


// object manager handles lifetime of stored object, no smart pointers here
class object_manager_t {
    protected:
    std::unique_ptr<abstract_object_factory_t> factory_;

    // created object pointers are stored in vector. Realocations are quite fine
    // when workign with "reasonable number" of object. Otherwise I'd consider probably a list
    std::vector<object_t*> objects_;
    mutable std::mutex mutex_;

    public:
    using ptr_t = std::shared_ptr<object_manager_t>;

    object_manager_t(std::unique_ptr<abstract_object_factory_t> obj_factory);
    virtual ~object_manager_t();

    template<typename object_type_e>
    object_t* create(tags_t tags, object_type_e type) {
        auto* new_obj = factory_->create(tags, static_cast<int>(type));
        TRACE() << "object_manager : created : " << (void*)new_obj << "\n";

        if (new_obj) {
            auto guard = std::lock_guard(mutex_);
            objects_.emplace_back(new_obj);
        }
        return new_obj;
    }

    void remove(object_t* obj);
    void accept(object_visitor_t* visitor);
    // calls cb on each stored object. Don't access other methods of object_managert_t
    // as its executed under a lock
    void for_each(std::function<void(object_t*)> cb);
    size_t size() const;
};