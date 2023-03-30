#include "object_manager.h"
#include "common.h"

#include <algorithm>

// ----- O B J E C T

object_t::object_t(tags_t tags, object_manager_t* man)
: tags_(std::move(tags))
, manager_(man)
{
    TRACE() << "object : create: " << (void*)this << "\n";
}

object_t::~object_t() {
    TRACE() << "object : destroy: " << (void*)this << "\n";
    if (manager_) {
        manager_->remove(this);
    }
}

bool object_t::test_tag(const std::string& name, const std::string& value) {
    TRACE() << "object: test_tag: k:" << name << " v:" << value << "\n";
    if (auto it = tags_.find(name); it != tags_.end()) {
        TRACE() << "object: test_tag: found: " << value << "\n";
        return it->second == value;
    }

    return false;
}


// ----- M A N A G E R

object_manager_t::object_manager_t(std::unique_ptr<abstract_object_factory_t> obj_factory)
: factory_(std::move(obj_factory))
, objects_()
{
    factory_->set_manager(this);
    TRACE() << "object_manager : created\n";
}

object_manager_t::~object_manager_t() {
    TRACE() << "object_manager : destroyed\n";
    auto guard = std::lock_guard(mutex_);
    for (auto ptr : objects_) {
        ptr->manager_ = nullptr; //prevent obj from deleting self
        delete ptr;
    }
}

void object_manager_t::accept(object_visitor_t* visitor) {
    TRACE() << "object_manager : accept\n";
    auto guard = std::lock_guard(mutex_);
    for (auto o : objects_)
        o->accept(visitor);
}

void object_manager_t::remove(object_t* obj) {
    TRACE() << "object_manager : remove: " << (void*)obj << "\n";
    auto guard = std::lock_guard(mutex_);
    auto it = std::remove_if(std::begin(objects_), std::end(objects_), [o=obj](auto i) { return o == i; });
    objects_.erase(it);
}

void object_manager_t::for_each(std::function<void(object_t*)> cb) {
    auto guard = std::lock_guard(mutex_);
    std::for_each(std::begin(objects_), std::end(objects_), cb);
}

size_t object_manager_t::size() const {
    auto guard = std::lock_guard(mutex_);
    return objects_.size();
}