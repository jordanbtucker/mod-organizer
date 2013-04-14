import os
#import mobase
import json
from Tkinter import *
import tkMessageBox
import ttk
import Tix


s_Settings = None
s_Organizer = None
s_Root = None


def type():
	return mobase.PluginType.tool

def init(organizer):
	global s_Settings
	global s_Organizer

	s_Organizer = organizer
	f = open(organizer.pluginDataPath() + "/settings_list.json", "r")
	s_Settings = json.loads(f.read())

	return True

def name():
	return "Py Ini Editor"

def author():
	return "Tannin"

def description():
	return "Ini Editor implemented in python"

def version():
	return mobase.VersionInfo(1, 0, 0, mobase.ReleaseType.beta)

def isActive():
	return True

def settings():
        
        
        return [ mobase.PluginSetting("TestString", "Test Description", "gaga"),
                 mobase.PluginSetting("Test", "Test Description", 5),
                 mobase.PluginSetting("TestBool", "Bool Description", True) ]


def displayName():
	return "Ini Editor"

def tooltip():
	return "Ini Editor"

def icon():
	return s_IconXPM;


def floatValidate(input):
        try:
                f = float(input)
                return True
        except:
                return False

def intValidate(input):
        try:
                i = int(input)
                return True
        except:
                return False

def addEntry(frame, sbar, settings, r, validation = None):
	t = Label(frame)
	if validation is None:
                e = Entry(frame)
        else:
                e = Entry(frame, validate="key", validatecommand=validation)
	t.configure(text = settings["key"])
	e.insert(0, settings["default"])
	t.grid(column=0, row=r, sticky=W)
	e.grid(column=2, row=r, sticky=E)
	b = Tix.Balloon(frame, statusbar=sbar)
	balloon = settings["description"] if "description" in settings else ""
	b.bind_widget(t, balloonmsg=balloon, statusmsg=balloon)
	b.bind_widget(e, balloonmsg=balloon, statusmsg=balloon)

def addCheckbox(frame, sbar, settings, r):
	c = Checkbutton(frame, text=settings["key"])
	c.grid(column=0, row=r, columnspan=2, sticky=W)
	b = Tix.Balloon(frame, statusbar=sbar)
	balloon = settings["description"] if "description" in settings else ""
	b.bind_widget(c, balloonmsg=balloon, statusmsg=balloon)

def sectionChanged(event):
	scrolled =  s_Root.nametowidget("settings_frame")
	statusbar = s_Root.nametowidget("status_bar")

	scrolled.grid_forget()
	scrolled.destroy()
	#frame = Frame(s_Root, name = "settings_frame")
	scrolled = Tix.ScrolledWindow(s_Root, scrollbar='auto', name = "settings_frame")
	scrolled.columnconfigure(1, weight=1)
	scrolled.pack()
	r = 0

        validate_float = (scrolled.window.register(floatValidate), '%S')
        validate_int = (scrolled.window.register(intValidate), '%S')

	for i in s_Settings[event.widget.get()]:
                keyType = i["key"][0]
		if keyType == 'b':
			addCheckbox(scrolled.window, statusbar, i, r)
                elif keyType == 'f':
                        addEntry(scrolled.window, statusbar, i, r, validate_float)
                elif keyType == 'i':
                        addEntry(scrolled.window, statusbar, i, r, validate_int)
		else:
			addEntry(scrolled.window, statusbar, i, r)
		r += 1

	scrolled.grid(row=1, sticky=N+E+W, padx=5, pady=5)

def display():
	global s_Root

	s_Root = Tix.Tk()
	s_Root.minsize(300,300)
        s_Root.geometry("500x500")
	s_Root.title("Ini Editor")

        section = StringVar()
        s_Root.columnconfigure(0, weight=1)
        s_Root.rowconfigure(1, weight=1)
	sectionBox = ttk.Combobox(s_Root, textvariable=section, state='readonly')
	sectionBox["value"] = s_Settings.keys()
	frame = Frame(s_Root, name = "settings_frame")
	sectionBox.grid(row=0, sticky=W)
	frame.grid(row=1, sticky=N+E+W)
	status = Tix.Label(s_Root, width = 40, name = "status_bar")
	status.grid(row=2, sticky=S+E+W)

	sectionBox.bind("<<ComboboxSelected>>", sectionChanged)
	s_Root.mainloop()


s_IconXPM = [   "22 22 65 1",
                " 	c None",
                ".	c #888A85",
                "+	c #878984",
                "@	c #FFFFFF",
                "#	c #DEDEDE",
                "$	c #E1E1E1",
                "%	c #E0E0E0",
                "&	c #E2E2E2",
                "*	c #E3E3E3",
                "=	c #E4E4E4",
                "-	c #E5E5E5",
                ";	c #DFDFDF",
                ">	c #B6B6B6",
                ",	c #B7B7B7",
                "'	c #E7E7E7",
                ")	c #E6E6E6",
                "!	c #E9E9E9",
                "~	c #EAEAEA",
                "{	c #ADAFAC",
                "]	c #515151",
                "^	c #A2A2A2",
                "/	c #EFEFEF",
                "(	c #ECECEC",
                "_	c #E8E8E8",
                ":	c #CECECE",
                "<	c #676767",
                "[	c #696969",
                "}	c #EDEDED",
                "|	c #C2C2C2",
                "1	c #565656",
                "2	c #ACACAC",
                "3	c #686868",
                "4	c #D0D0D0",
                "5	c #6A6A6A",
                "6	c #D6D6D6",
                "7	c #F1F1F1",
                "8	c #F0F0F0",
                "9	c #EBEBEB",
                "0	c #6B6B6B",
                "a	c #6C6C6C",
                "b	c #D9D9D9",
                "c	c #F2F2F2",
                "d	c #AEAFAD",
                "e	c #5F5F5F",
                "f	c #838383",
                "g	c #EEEEEE",
                "h	c #D7D7D7",
                "i	c #787878",
                "j	c #858585",
                "k	c #939393",
                "l	c #9F9F9F",
                "m	c #B0B0B0",
                "n	c #757575",
                "o	c #A0A0A0",
                "p	c #ADADAD",
                "q	c #F4F4F4",
                "r	c #B1B1B1",
                "s	c #BDBDBD",
                "t	c #F5F5F5",
                "u	c #F6F6F6",
                "v	c #F8F8F8",
                "w	c #F7F7F7",
                "x	c #F3F3F3",
                "y	c #F9F9F9",
                "z	c #868883",
                "                      ",
                "  .................+  ",
                "  .@@@@@@@@@@@@@@@@.  ",
                "  .@#$%$&&*===---=@.  ",
                "  .@;&$>>,,,,,,,')@.  ",
                "  .@%*&&)))*''''''@.  ",
                "  .@$==>>>>>>>>>'!@.  ",
                "  .@&)---~~~~~~~~~@.  ",
                "  .@{))>>>>]]]^>/(@.  ",
                "  .@=@'____:<[[}/}@.  ",
                "  .@-'_||12|211|//@.  ",
                "  .@)_!~~334355678@.  ",
                "  .@)!!9(5500aaabc@.  ",
                "  .@d!~||211111ef#@.  ",
                "  .@'@9~~(g//hijkl@.  ",
                "  .@_~9|||||||mnop@.  ",
                "  .@_~9988887cq=rs@.  ",
                "  .@_99|||||||tuvw@.  ",
                "  .@_99}g8cxtuvyvu@.  ",
                "  .@@@@@@@@@@@@@@@@.  ",
                "  z................   ",
                "                      " ]
