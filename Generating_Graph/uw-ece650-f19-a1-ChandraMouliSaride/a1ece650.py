from __future__ import  print_function
import sys
import re
import math
# YOUR CODE GOES HERE
class Point:
  def __init__(self, x, y):
    self.x = x
    self.y = y
#Used function directly from internet for finding distance between coordinates
def calculateDistance(x1, y1, x2, y2):
    dist = math.sqrt((x2 - x1) ** 2 + (y2 - y1) ** 2)
    return dist
#Based on articles from internet , modified for our requirement
def checkbound(A,B,C):
    return (C.y-A.y)*(B.x-A.x) > (B.y-A.y)*(C.x-A.x)
def limitbound(p1,p2,p3,p4):
    return checkbound(p1,p3,p4) != checkbound(p2,p3,p4) and checkbound(p1,p2,p3) != checkbound(p1,p2,p4)
def intersection(p1,p2,p3,p4):
    if(limitbound(p1,p2,p3,p4)):

        a1 = p2.y -p1.y

        b1 =p1.x-p2.x;

        c1 = a1 * p1.x + b1 * (p1.y);


        a2 = p4.y-p3.y;
        b2 =p3.x-p4.x;
        c2 = a2 * (p3.x) + b2 * (p3.y);


        determinant = a1 * b2 - a2 * b1;
        if(determinant!=0):
            x = float((b2 * c1 - b1 * c2)) / float(determinant);
            y = float((a1 * c2 - a2 * c1)) / float(determinant);
            final_x = 0
            final_y = 0
            if (type(x) is float):
                x_cord = float(x)
                final_x = float("{0:.2f}".format(x_cord))
            if (type(y) is float):
                y_cord = float(y)
                final_y = float("{0:.2f}".format(y_cord))
            if (type(x) is int):
                x_cord = int(x)
                final_x = x_cord
            if (type(y) is int):
                y_cord = int(y)
                final_y = y_cord
            p=Point(final_x,final_y)
            return p
        else:
            return None
    else:
        return  None
def add_street(line,street_data):
    ip = line.split('"')
    if(len(ip[2].lstrip())==len(ip[2])):
        raise Exception("Please give space between street and coordinates as per format")
        return street_data
    tot = ip[2].replace(" ", "")
    coord = re.findall("\((.*?)\)", tot)
    brk_chk = re.findall(r'-?\d+\.?\d*', tot)
    if not(float(len(coord))==float(len(brk_chk)/2.0)):
        raise Exception("Please check and give proper format for coordinates")
        return street_data
    st_keys = street_data.keys()
    do_flag = True
    for chk in st_keys:
        if ip[1].lower() == chk.lower():
            raise Exception("'a' is specified for street that alreaady exists")
            do_flag = False
            break

    if (do_flag):

        coord_list = []
        if(len(coord)>1):

            for i in coord:
                i_split = i.split(',')
                final_x=0
                final_y=0
                x_cord_li = re.findall(r'-?\d+\.?\d*', i_split[0])
                y_cord_li = re.findall(r'-?\d+\.?\d*', i_split[1])
                if(x_cord_li[0].__contains__('.')):
                    x_cord = float(x_cord_li[0])
                    final_x=float("{0:.2f}".format(x_cord))
                else:
                    x_cord = int(x_cord_li[0])
                    final_x = x_cord
                if(y_cord_li[0].__contains__('.')):
                    y_cord = float(y_cord_li[0])
                    final_y=float("{0:.2f}".format(y_cord))
                else:
                    y_cord = int(y_cord_li[0])
                    final_y=y_cord
                p = Point(final_x,final_y)
                coord_list.append(p)

            street_data.update({ip[1].lower(): coord_list})
        else:
            raise Exception("'a' is specified without minimum needed coordinates")
    return street_data
def remove_street(street_data,line):
    st_keys = street_data.keys();
    ip = line.split('"')
    do_flag = True
    for chk in st_keys:
        if ip[1].lower() == chk.lower():
            do_flag = False
            street_data.pop(ip[1].lower())
            vertex_data = {}
            break
    if (do_flag):
        raise Exception("'r' specified for a street that does not exist")
    return  street_data
def change_street(line,street_data):
    ip = line.split('"')
    if(len(ip[2].lstrip())==len(ip[2])):
        raise Exception("Please give space between street and coordinates as per format")
        return street_data
    tot = ip[2].replace(" ", "")
    coord = re.findall("\((.*?)\)", tot)
    brk_chk = re.findall(r'-?\d+\.?\d*', tot)
    if not (float(len(coord))==float(len(brk_chk)/2.0)):
        raise Exception("Please check and give proper format for coordinates")
        return street_data
    st_keys = street_data.keys()
    do_flag = False
    for chk in st_keys:
        if ip[1].lower() == chk.lower():
            do_flag = True
            break

    if (do_flag):

        coord_list = []
        if (len(coord) > 1):
            for i in coord:
                i_split = i.split(',')
                final_x = 0
                final_y = 0
                x_cord_li = re.findall(r'-?\d+\.?\d*', i_split[0])
                y_cord_li = re.findall(r'-?\d+\.?\d*', i_split[1])
                if (x_cord_li[0].__contains__('.')):
                    x_cord = float(x_cord_li[0])
                    final_x = float("{0:.2f}".format(x_cord))
                else:
                    x_cord = int(x_cord_li[0])
                    final_x = x_cord
                if (y_cord_li[0].__contains__('.')):
                    y_cord = float(y_cord_li[0])
                    final_y = float("{0:.2f}".format(y_cord))
                else:
                    y_cord = int(y_cord_li[0])
                    final_y = y_cord
                p = Point(final_x,final_y)

                coord_list.append(p)

            street_data.update({ip[1].lower(): coord_list})
            vertex_data = {}
        else:
            raise Exception("'c' is specified without minimum needed coordinates")

    else:
        raise Exception("'c' specified for a street that does not exisr")
    return street_data
def vertex_gen(street_data):
    p_id = 1
    vertex_data={}
    elist = []
    for chk_street in street_data.keys():
        for tmp_street in street_data.keys():
            if (chk_street != tmp_street):
                points_list1 = street_data[chk_street]
                points_list2 = street_data[tmp_street]
                for i in range(len(points_list1)):
                    for j in range(len(points_list2)):
                        if (i < len(points_list1) - 1 and j < len(points_list2) - 1):

                            p1 = points_list1[i]
                            p2 = points_list1[i + 1]
                            p3 = points_list2[j]
                            p4 = points_list2[j + 1]
                            pint = intersection(p1, p2, p3, p4)
                            cp1=False
                            cp2=False
                            cp3=False
                            cp4=False
                            cp1id=0
                            cp2id=0
                            cp3id=0
                            cp4id=0
                            cpintid=0
                            if (pint != None):
                                rep = False
                                for p in vertex_data.keys():
                                    if (vertex_data[p] == (p1.x,p1.y)):
                                        rep = True
                                        break
                                if (rep == False):
                                    vertex_data[p_id] = (p1.x,p1.y)
                                    cp1id=p_id
                                    p_id = p_id + 1
                                    cp1=True
                                rep = False
                                for p in vertex_data.keys():
                                    if (vertex_data[p] == (p2.x,p2.y)):
                                        rep = True
                                        break
                                if (rep == False):
                                    vertex_data[p_id] = (p2.x,p2.y)
                                    cp2id=p_id
                                    p_id = p_id + 1
                                    cp2=True
                                rep = False
                                for p in vertex_data.keys():
                                    if (vertex_data[p] == (p3.x,p3.y)):
                                        rep = True
                                        break
                                if (rep == False):
                                    vertex_data[p_id] = (p3.x,p3.y)
                                    cp3id=p_id
                                    p_id = p_id + 1
                                    cp3=True
                                rep = False
                                for p in vertex_data.keys():
                                    if (vertex_data[p] == (p4.x,p4.y)):
                                        rep = True
                                        break
                                if (rep == False):
                                    vertex_data[p_id] = (p4.x,p4.y)
                                    cp4id=p_id
                                    p_id = p_id + 1
                                    cp4=True

                                rep = False
                                for p in vertex_data.keys():
                                    if (vertex_data[p] == (pint.x,pint.y)):
                                        rep = True
                                        cpintid=p
                                        break
                                if (rep == False):
                                    vertex_data[p_id] = (pint.x,pint.y)
                                    cpintid=p_id
                                    p_id = p_id + 1
                                if(cp1):
                                    elist.append((cp1id, cpintid))
                                else:
                                    for p in vertex_data.keys():
                                        if (vertex_data[p] == (p1.x, p1.y)):
                                            cp1id=p
                                            break
                                    if(cp1id is not 0):
                                        elist.append((cp1id, cpintid))
                                if(cp2):
                                    elist.append((cp2id, cpintid))
                                else:
                                    for p in vertex_data.keys():
                                        if (vertex_data[p] == (p2.x, p2.y)):
                                            cp2id=p
                                            break
                                    if (cp2id is not 0):
                                        elist.append((cp2id, cpintid))
                                if(cp3):
                                    elist.append((cp3id, cpintid))
                                else:
                                    for p in vertex_data.keys():
                                        if (vertex_data[p] == (p3.x, p3.y)):
                                            cp3id=p
                                            break
                                    if (cp3id is not 0):
                                        elist.append((cp3id, cpintid))
                                if(cp4):
                                    elist.append((cp4id, cpintid))
                                else:
                                    for p in vertex_data.keys():
                                        if (vertex_data[p] == (p4.x, p4.y)):
                                            cp4id
                                            break
                                    if (cp4id is not 0):
                                        elist.append((cp4id, cpintid))

    fin_list=list(set(elist))
    edges_list=find_edges(vertex_data,fin_list)
    return vertex_data,edges_list

def find_edges(V,elist):
    redundant_list = []
    first_point = set()
    for i in elist:
        cnt = 0
        for j in elist:
            if (i[0] == j[0]):
                cnt = cnt + 1
        if (cnt > 1):
            redundant_list.append(i)
            first_point.add(i[0])
    if(len(redundant_list)>0):

        for i in redundant_list:
            if i in elist:
                elist.remove(i)
        intersections_list = []
        for i in first_point:
            tmp_list = [b for a, b in redundant_list if a == i]
            p1 = V.get(i)
            point_list = []
            for p in tmp_list:
                point_list.append(V.get(p))
            dist_list = []
            final_point = ()
            for p in point_list:
                dist = calculateDistance(p1[0], p1[1], p[0], p[1])
                dist_list.append(dist)
                if (min(dist_list) == dist):
                    final_point = p
            intersections_list.append(final_point)
            src = [a for a, b in V.items() if b == p1]
            dst = [a for a, b in V.items() if b == final_point]
            elist.append((src[0], dst[0]))
        last_list = []
        for p1 in intersections_list:
            dist = 0
            dist_list = []
            final_point = ()
            for p2 in intersections_list:
                if not (p1 == p2):
                    dist = calculateDistance(p1[0], p1[1], p2[0], p2[1])
                    dist_list.append(dist)
                    if (min(dist_list) == dist):
                        final_point = p2
            src = [a for a, b in V.items() if b == p1]
            dst = [a for a, b in V.items() if b == final_point]
            last_list.append((src[0], dst[0]))
        tmp_list = last_list
        for i in tmp_list:
            for j in tmp_list:
                if (i[0] == j[1] and i[1] == j[0]):
                    last_list.remove(i)
        for i in last_list:
            elist.append(i)
    return elist


def main():
    ### YOUR MAIN CODE GOES HERE

    ### sample code to read from stdin.
    ### make sure to remove all spurious print statements as required
    ### by the assignment
    street_data = {}
    lines_data={}
    vertex_data={}
    elist=[]
    while True:
        try:
            line = sys.stdin.readline()
            if line=='':
            	break
            sp_chk=line.split()
            if (len(sp_chk)==0):
                raise Exception('Please provide inputs as per format')
            if (len(sp_chk[0])>1):
                raise Exception('Please provide inputs as per format')
            ip = line.split('"')
            if(len(ip)==0):
                raise Exception('Please provide inputs as per format')
            cmd = ip[0].strip()
            if line == '':
                break
            if cmd =='a':
                street_data=add_street(line,street_data)
                vertex_data={}
            elif cmd =='r':
                street_data=remove_street(street_data,line)
                vertex_data={}

            elif cmd =='c':
                street_data=change_street(line,street_data)
                vertex_data={}

            elif cmd=='g':
                vertex_data,elist=vertex_gen(street_data)


                print(('V = {'),file=sys.stdout)
                for i in vertex_data.keys():
                    print(" "+str(i)+":  "+str(vertex_data[i]),file=sys.stdout)
                print('}',file=sys.stdout)
                print(('E = {'), file=sys.stdout)
                chk_cnt=len(elist)
                tmp_cnt=0
                for i in elist:
                    tmp_cnt=tmp_cnt+1
                    if not(tmp_cnt==chk_cnt):
                        print("  <"+str(i[0])+","+str(i[1])+">,", file=sys.stdout)
                    else:
                        print("  <" + str(i[0]) + "," + str(i[1]) + ">", file=sys.stdout)
                print('}', file=sys.stdout)
            else:
                raise Exception('Please enter proper commands to recognise')
        except Exception as exp:
                print('Error: '+str(exp), file=sys.stderr)





    # return exit code 0 on successful termination
    sys.exit(0)


if __name__ == '__main__':
    main()


