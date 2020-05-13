## A simple unit test example. Replace by your own tests

import sys
import unittest
import a1ece650
class MyTest(unittest.TestCase):

    def test_one(self):
       """Test for checking calculateDistance function """
       self.assertEqual(a1ece650.calculateDistance(4,2,4,4),2)

    def test_two(self):
    	"""Test for checking intersection function"""
    	p1=a1ece650.Point(2,2)
    	p2=a1ece650.Point(5,5)
    	p3=a1ece650.Point(4,2)
    	p4=a1ece650.Point(4,8)
    	self.assertEqual(a1ece650.intersection(p1,p2,p3,p4).x,4)
    	self.assertEqual(a1ece650.intersection(p1,p2,p3,p4).y,4)

    def test_three(self):
        """Test for checking intersection function"""
        p1=a1ece650.Point(2,2)
    	p2=a1ece650.Point(5,5)
    	p3=a1ece650.Point(3,8)
    	p4=a1ece650.Point(5,6)
    	self.assertEqual(a1ece650.intersection(p1,p2,p3,p4),None)

    def test_four(self):
        """Test for checking add street function"""
        p1=a1ece650.Point(4,2)
    	p2=a1ece650.Point(4,8)
    	coord_list=[]
    	coord_list.append(p1)
    	coord_list.append(p2)
    	street_data={"King Street S".lower():coord_list}
    	street_data_from=a1ece650.add_street("a \"Davenport Road\" (1,4)(5,8)",street_data)
    	p1=a1ece650.Point(1,4)
    	p2=a1ece650.Point(5,8)
    	coord_list=[]
    	coord_list.append(p1)
    	coord_list.append(p2)
    	street_data.update({"Davenport Road".lower():coord_list})
    	self.assertEqual(street_data.keys(),street_data_from.keys())
    
    def test_five(self):
        """Test for checking add street function"""
        p1=a1ece650.Point(4.5,2.5)
    	p2=a1ece650.Point(4.5,8.4)
    	coord_list=[]
    	coord_list.append(p1)
    	coord_list.append(p2)
    	street_data={"King Street S".lower():coord_list}
    	street_data_from=a1ece650.add_street("a \"Davenport Road\" (1.4,4.4)(5.4,8.4)",street_data)
    	p1=a1ece650.Point(1.4,4.4)
    	p2=a1ece650.Point(5.4,8.4)
    	coord_list=[]
    	coord_list.append(p1)
    	coord_list.append(p2)
    	street_data.update({"Davenport Road".lower():coord_list})
    	self.assertEqual(street_data.keys(),street_data_from.keys())
    
    def test_six(self):
        """Test for checking add street function"""
        p1=a1ece650.Point(4.5,2.5)
    	p2=a1ece650.Point(4.5,8.4)
    	coord_list=[]
    	coord_list.append(p1)
    	coord_list.append(p2)
    	street_data={"King Street S".lower():coord_list}
    	with self.assertRaises(Exception):
    		a1ece650.add_street("a \"King Street S\" (1.4,4.4)(5.4,8.4)",street_data)

    def test_seven(self):
        """Test for checking add street function"""
    	street_data={}
    	with self.assertRaises(Exception):
    		a1ece650.add_street("a \"King Street S\"(1.4,4.4)(5.4,8.4)",street_data)
    
    def test_eight(self):
        """Test for checking add street function"""
    	street_data={}
    	with self.assertRaises(Exception):
    		a1ece650.add_street("a \"King Street S\" (1.4,4.4)(5.4,8.4",street_data)
    
    def test_nine(self):
        """Test for checking add street function"""
    	street_data={}
    	with self.assertRaises(Exception):
    		a1ece650.add_street("a \"King Street S\" (1.4,4.4)",street_data)
    
    def test_ten(self):
        """Test for checking remove street function"""
    	street_data={}
    	with self.assertRaises(Exception):
    		a1ece650.remove_street(street_data,"r \"King Street S\" ")
    
    def test_eleven(self):
        """Test for checking remove street function"""
    	street_data={}
    	p1=a1ece650.Point(4.5,2.5)
    	p2=a1ece650.Point(4.5,8.4)
    	coord_list=[]
    	coord_list.append(p1)
    	coord_list.append(p2)
    	street_data={"King Street S".lower():coord_list}
    	street_data_from=a1ece650.remove_street(street_data,"r \"King Street S\"")
    	self.assertEqual(street_data.keys(),street_data_from.keys())

    def test_twelve(self):
        """Test for checking change street function"""
        p1=a1ece650.Point(4,2)
    	p2=a1ece650.Point(4,8)
    	coord_list=[]
    	coord_list.append(p1)
    	coord_list.append(p2)
    	street_data={"King Street S".lower():coord_list}
    	street_data_from=a1ece650.change_street("c \"King Street S\" (1,4)(5,8)",street_data)
    	self.assertEqual(street_data.keys(),street_data_from.keys())

    def test_thirteen(self):
        """Test for checking change street function"""
        p1=a1ece650.Point(4,2)
    	p2=a1ece650.Point(4,8)
    	coord_list=[]
    	coord_list.append(p1)
    	coord_list.append(p2)
    	street_data={"King Street S".lower():coord_list}
    	street_data_from=a1ece650.change_street("c \"King Street S\" (1.5,4.5)(5.5,8.5)",street_data)
    	self.assertEqual(street_data.keys(),street_data_from.keys())

    def test_fourteen(self):
        """Test for checking change street function"""
    	street_data={}
    	with self.assertRaises(Exception):
    		a1ece650.change_street("c \"King Street S\" (1.4,4.4)(5.4,8.4)",street_data)

    def test_fifteen(self):
        """Test for checking change street function"""
    	street_data={}
    	with self.assertRaises(Exception):
    		a1ece650.change_street("c \"King Street S\" (1.4,4.4)(5.4,8.4",street_data)

    def test_sixteen(self):
        """Test for checking change street function"""
    	street_data={}
    	with self.assertRaises(Exception):
    		a1ece650.change_street("c \"King Street S\"(1.4,4.4)(5.4,8.4)",street_data)
    
    def test_seventeen(self):
        """Test for checking change street function"""
    	p1=a1ece650.Point(4,2)
    	p2=a1ece650.Point(4,8)
    	coord_list=[]
    	coord_list.append(p1)
    	coord_list.append(p2)
    	street_data={"King Street S".lower():coord_list}
    	with self.assertRaises(Exception):
    		a1ece650.change_street("c \"King Street S\" (1.4,4.4)",street_data)

    def test_eighteen(self):
        """Test for checking vertex_gen street function"""
        street_data={}
    	street_data=a1ece650.add_street("a \"Weber Street\" (2,-1)(2,2)(5,5)(5,6)(3,8)",street_data)
    	street_data=a1ece650.add_street("a \"King Street S\" (4,2)(4,8)",street_data)
    	street_data=a1ece650.add_street("a \"Davenport Road\" (1,4)(5,8)",street_data)
    	vertex_data,elist=a1ece650.vertex_gen(street_data)
    	self.assertEqual(len(elist),9)
    	self.assertEqual(len(vertex_data.keys()),10)

    def test_nineteen(self):
        """Test for checking vertex_gen street function"""
        street_data={}
    	street_data=a1ece650.add_street("a \"Weber Street\" (2,1)(2,2)",street_data)
    	street_data=a1ece650.add_street("a \"King Street S\" (4,2)(4,8)",street_data)
    	street_data=a1ece650.add_street("a \"Davenport Road\" (1,4)(5,8)",street_data)
    	vertex_data,elist=a1ece650.vertex_gen(street_data)
    	self.assertEqual(len(elist),4)
    	self.assertEqual(len(vertex_data.keys()),5)

    def test_twenty(self):
        """Test for checking vertex_gen street function"""
        street_data={}
    	street_data=a1ece650.add_street("a \"Weber Street\" (2,1)(2,2)",street_data)
    	street_data=a1ece650.add_street("a \"King Street S\" (4,2)(4,8)",street_data)
    	street_data=a1ece650.add_street("a \"Davenport Road\" (1,4)(5,8)",street_data)
    	street_data=a1ece650.remove_street(street_data,"r \"King Street S\"")
    	vertex_data,elist=a1ece650.vertex_gen(street_data)
    	self.assertEqual(len(elist),0)
    	self.assertEqual(len(vertex_data.keys()),0)



if __name__ == '__main__':
    unittest.main()
