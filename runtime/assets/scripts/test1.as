class Test1 {
    engine::ActorId id;

    Test1(engine::ActorId _id) {
        id = _id;
        print(engine::toString(id));
    }

    void process() {
        // print("Howdy!");

        engine::Transform trans = engine::getBoundTransform(); // Gets current transform
        trans.position.x += 0.01f; // Changes position
        engine::setBoundTransform(trans); // Reapply to current actor
    }
}