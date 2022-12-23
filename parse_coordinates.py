input = "15:11:52.971 $GPGGA,141153.000,4803.6142,N,01136.8491,E,2,9,0.92,550.1,M,47.PGSV,3,3,11,10,16,291,23,32,13,320,31,23,11,258,33*4A"

def parse_line(s):
    contents = s.split("$")
    for i in range(1, len(contents)):
        if contents[i].split(",")[0] == "GPGGA":
            return parse_gpgga(contents[i])
        else:
            return []

def parse_gpgga(s):
    contents = s.split(",")
    out = [contents[2], contents[3], contents[4], contents[5]]
    out[0] = int(out[0].split(".")[0][:-2]) + float(out[0].split(".")[0][-2:] + "." + out[0].split(".")[1]) / 60
    out[2] = int(out[2].split(".")[0][:-2]) + float(out[2].split(".")[0][-2:] + "." + out[2].split(".")[1]) / 60
    return out

def create_gpx(l):
    out = ['<?xml version="1.0" encoding="UTF-8"?>']
    out.append('<gpx version="1.1" creator="Jan">')
    out.append("<trk>")
    out.append("<name>Track1</name>")
    out.append("<trkseg>")

    for e in l:
        out.append('<trkpt lat="{}" lon="{}">'.format(e[0], e[2]))
        out.append("</trkpt>")

    out.append("</trkseg>")
    out.append("</trk>")
    out.append("</gpx>")

    return out

def parse_file(name):
    file = open(name)
    coordinates = []
    for line in file:
        l = parse_line(line)
        if l != [] and l != None:
            coordinates.append(l)
    print("".join(create_gpx(coordinates)))
    with open("out.gpx", 'w') as fp:
        for item in create_gpx(coordinates):
            fp.write("%s\n" % item)

if __name__ == "__main__":
    # create_file([["4803.6142","N","01136.8491","E"],["4803.6142","N","01136.8491","E"]])
    parse_file("TestLogFuerJan.txt")
