function main() {
    system.println("WOOOWW");
    //system.println(add(4, 10));
    system.println("Hello World from main");
    system.print("Hello");
    system.println("World");

    for(let i = 0; i < 10; i++) {
        system.print(i, ' ');
    }

    system.println();

    let file = system.open("test.txt", system.FILE_WRITE_ONLY | system.FILE_CREATE | system.FILE_TRUNCATE);
    system.println("File:", file);

    let result = system.write(file, "Hello World");
    if(result == -1)
    {
        system.println("Error while writing to file");
    }

    system.close(file);

    system.test = 123;
    system.println(system.test);

    //hello = 321;
    system.println(hello);

    obj = {};
    obj["test"] = "Hello World";

    system.println(obj.test);
}