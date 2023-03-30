#include "common.h"
#include "entity.h"
#include "object_manager.h"
#include "entity_actions_implementation.h"

#include <iostream>
#include <memory>
#include <algorithm>


int main(int argc, char* argv[]) {
    TRACE() << "Hello world of entities!\n";

    entity_container_t entities; //just to fullfil requirements

    TRACE() << "=== prepare entity1\n";
    auto obj_factory1 = std::unique_ptr<abstract_object_factory_t>(new action_one_factory_t());
    auto object_man1 = std::make_shared<object_manager_t>(std::move(obj_factory1));
    entities.emplace_back(object_man1);


    TRACE() << "=== prepare entity2\n";
    auto obj_factory2 = std::unique_ptr<abstract_object_factory_t>(new action_two_factory_t());
    auto object_man2 = std::make_shared<object_manager_t>(std::move(obj_factory2));
    entities.emplace_back(object_man2);

    // --- Entity1 ---
    TRACE() << "=== fill entity1 with some components in tree hiearchy\n";
    auto& ent1 = entities.front();
    auto e1_c1 = ent1.new_component({{"name", "e1c1"}});
    auto e1_c2 = ent1.new_component({{"name", "e1c2"}});
    auto e1_c2_c1 = e1_c2->new_child({{"name", "e1c2.c1"}});
    auto e1_c2_c1_c1 = e1_c2->new_child({{"name", "e1c2.c1.c1"}});

    TRACE() << "=== crate custom actions on components\n";
    // no need to keep pointer to objects, but nvm
    // visible obj_e1c2, obj_e1c1.c1.c1
    object_t* tmp = e1_c1->new_object({{"name", "obj_e1c1"}, {"visible", "false"}, {"type", "run"}}, actions1_e::run);
    tmp = e1_c2->new_object({{"name", "obj_e1c2"}, {"visible", "true"}, {"type", "run"}}, actions1_e::run);
    tmp = e1_c2_c1->new_object({{"name", "obj_e1c2.c1"}, {"visible", "false"}, {"type", "crouch"}}, actions1_e::crouch);
    tmp = e1_c2_c1_c1->new_object({{"name", "obj_e1c1.c1.c1"}, {"visible", "true"}, {"type", "crouch"}}, actions1_e::crouch);


    TRACE() << "=== call custom non-virtual actions on actions in entity1 - visitor approach\n";
    auto actions_one_nonvirt_visitor = create_action_one_nonvirtual_visitor();
    ent1.get_object_manager()->accept(actions_one_nonvirt_visitor.get());

    TRACE() << "=== collect all 'visible' actions on entity1 - visitor approach\n";
    action_one_visible_objects_collector_t visible_actions_visitor{};
    ent1.get_object_manager()->accept(&visible_actions_visitor);

    TRACE() << "=== collected objects: " << visible_actions_visitor.visible_actions.size() << "\n";
    std::for_each(std::begin(visible_actions_visitor.visible_actions), std::end(visible_actions_visitor.visible_actions),
                    [](const auto& o) {
                        TRACE() << "\t" << *o;
                    });

    // --- Entity2 ---
    TRACE() << "=== fill entity2 with some components in tree hiearchy and some objects from 'cathegory' actions2\n";
    auto& ent2 = entities.back();
    ent2.new_component({{"name", "e2c1"}})
        ->new_child({{"name", "e2c1.c1"}})
            ->new_object({{"name", "obj_e2c1.c1"}}, actions2_e::die)
                ->get_tags()["visible"] = "true";

    ent2.new_component({{"name", "e2c2"}})
        ->new_child({{"name", "e2c2.c1"}})
            ->new_object({{"name", "obj_e2c2.c1"}}, actions2_e::fire)
                ->get_tags()["visible"] = "true";

    auto to_detach_obj = ent2.new_component({{"name", "e2c3"}})
        ->new_object({{"name", "obj_e2c3"}, {"visible", "true"}}, actions2_e::fire);

    TRACE() << "=== ent2 stored objects: " << ent2.get_object_manager()->size() << "\n";
    ent2.get_object_manager()->remove(to_detach_obj);
    TRACE() << "=== ent2 stored objects after detach: " << ent2.get_object_manager()->size() << "\n";
    TRACE() << "=== ent2 detached object : (e.g. is still valid) " << *to_detach_obj;

    TRACE() << "=== collect all 'visible' actions on entity1 - 'manual' approach\n";
    std::vector<object_t*> ent2_visible_objects;
    ent2.get_object_manager()->for_each(
        [&ent2_visible_objects](auto o) {
            if (o->test_tag("visible", "true"))
                ent2_visible_objects.push_back(o);
        });
    TRACE() << "=== collected objects: " << ent2_visible_objects.size() << "\n";
    std::for_each(std::begin(ent2_visible_objects), std::end(ent2_visible_objects),
                    [](const auto& o) {
                        TRACE() << "\t" << *o;
                    });


    return 0;
}