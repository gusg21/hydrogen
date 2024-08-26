class Test1 {
    engine::ActorId id;

    Test1(engine::ActorId _id) {
        id = _id;
        print(engine::toString(id));
    }

    void process() {
        // print("Howdy!");

        engine::Transform trans = engine::getBoundTransform(); // Gets current transform
        trans.pos.x += 0.01f;
        engine::setBoundTransform(trans);
    }
}