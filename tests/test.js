function main(args) {
    //let test = importModule("Test");
    //test.wow();

    let mod = importModule("testModule");
    console.info(mod.addFunc(4, 10));

}