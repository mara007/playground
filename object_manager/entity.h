#pragma once

#include "common.h"
#include "object_manager.h"

#include <vector>
#include <memory>

//fwd
class entity_t;

class component_t : public std::enable_shared_from_this<component_t> {
    public:
    using ptr_t = std::shared_ptr<component_t>;

    // creates a new child at this level of component hiearchy
    ptr_t new_child(tags_t tags);

    // allows component to create a new object of a desired type. Use the same enum as was used with factory
    // just to fullfill requirements - anyone with access to object_manager can create objects
    template<typename obj_type_e>
    object_t* new_object(tags_t tags, obj_type_e type) {
        return manager_->create(tags, type);
    }

    virtual ~component_t();

    private:
    //I want this class to be instantiable only via `new_child()` and entity_t which creates a root component
    friend class entity_t;
    component_t(object_manager_t::ptr_t manager, ptr_t parent, tags_t tags);
    component_t() = delete;
    component_t(const component_t&) = delete;

    std::vector<ptr_t> child_components_;
    ptr_t parent_component_;
    object_manager_t::ptr_t manager_;

    tags_t tags_;
};

// top-level view of a collections of an arbitrary components
class entity_t {
    object_manager_t::ptr_t manager_;
    component_t::ptr_t root_component_;

    public:
    using ptr_t = std::shared_ptr<entity_t>;

    entity_t(object_manager_t::ptr_t obj_man);
    virtual ~entity_t() = default;

    object_manager_t::ptr_t get_object_manager();
    component_t::ptr_t new_component(tags_t tags);
};

using entity_container_t = std::vector<entity_t>;