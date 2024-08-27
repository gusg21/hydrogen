class Test1 {
    engine::ActorId id;

    Test1(engine::ActorId _id) {
        id = _id;
        print(engine::toString(id));
    }

    void process() {
        // print("Howdy!");

        engine::Transform trans = engine::getBoundTransform(); // Gets current transform
        trans.rotation = engine::rotate(trans.rotation, 0.5f, engine::newVec3(0.f, 1.f, 0.f)); // Changes position
        engine::setBoundTransform(trans); // Reapply to current actor
    }
}