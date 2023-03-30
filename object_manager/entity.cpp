#include "entity.h"
#include "common.h"


// ----- C O M P O N E N T

component_t::component_t(object_manager_t::ptr_t manager, ptr_t parent, tags_t tags)
: child_components_()
, parent_component_(parent)
, manager_(manager)
, tags_(std::move(tags))
{
    TRACE() << "component_t: created: " << (void*)this << "\n";
}


component_t::~component_t() {
    TRACE() << "component_t: destroyed: " << (void*)this << "\n";
}


component_t::ptr_t component_t::new_child(tags_t tags) {
    TRACE() << "component_t: new_child\n";
    auto new_c = std::shared_ptr<component_t>(new component_t(manager_, shared_from_this(), std::move(tags)));
    child_components_.emplace_back(new_c);
    return new_c;
}

// ----- E N T I T Y

entity_t::entity_t(object_manager_t::ptr_t obj_man)
: manager_(obj_man)
, root_component_(new component_t(manager_, nullptr, {{"name", "root_component"}}))
{}

object_manager_t::ptr_t entity_t::get_object_manager() {
    return manager_;
}

component_t::ptr_t entity_t::new_component(tags_t tags) {
    TRACE() << "entity_t : new_component\n";
    return root_component_->new_child(std::move(tags));
}
