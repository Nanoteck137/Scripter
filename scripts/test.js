console.assert(false, "Hello World");

function main(args) {
    /*system.println(args);
    let file = system.open("test.txt", system.FILE_WRITE_ONLY | system.FILE_CREATE | system.FILE_TRUNCATE);

    system.println("File:", file);

    let result = system.write(file, "Hello World");

    if (result == -1) {
        system.println("Error while writing to file");
    }

    system.close(file);*/

    //test.wow();
    system.println("Hello World");

    console.info("Hello World");
    console.warning("Hello World");
    console.error("Hello World");
    console.critical("Hello World");

    importModule("Test");

    test.wow();

}