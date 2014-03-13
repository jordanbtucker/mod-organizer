function zlibLibraryPaths(qbs) {
    if (  File.exists(qbs.getenv("ZLIBPATH") + "/zlibstat.lib") ||
          File.exists(qbs.getenv("ZLIBPATH") + "/zlibstatic.lib")) {
        return [ qbs.getenv("ZLIBPATH") ];
    } else {
        return [ qbs.getenv("ZLIBPATH") + "/build" ];
    }
}
function zlibLibs(qbs) {
    if (  File.exists(qbs.getenv("ZLIBPATH") + "/zlibstat.lib") ||
          File.exists(qbs.getenv("ZLIBPATH") + "build/zlibstat.lib")) {
        return [ 'zlibstat' ];
    } else {
        return [ 'zlibstatic' ];
    }
}

