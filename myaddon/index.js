const bindings = require('bindings');
var addon = bindings('myaddon');

addon.print(process.argv[2]);
