import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

class SignalHandler:
	def __init__(self):
		self.option_A = False
		self.option_F = False
		self.option_U = False
		self.option_L = False
		self.option_X = False
		self.option_E = False

	def onDestroy(self, *args):
		Gtk.main_quit()

	def on_ExecuteButton_clicked(self, ExecuteButton):
		psArgs = "-"

		if(self.option_A):
			psArgs += "A"

		if(self.option_F):
			psArgs += "F"

		if(self.option_U):
			psArgs += "U"
		
		if(self.option_L):
			psArgs += "L"

		if(self.option_X):
			psArgs += "X"

		if(self.option_E):
			psArgs += "E"

		print("Argumenty:" + psArgs)

	def on_Basic_clicked(self, Basic):
		PSArgs.set_opacity(0.0)
		OptionsCheckBoxes.set_opacity(1.0)

	def on_CommandLine_clicked(self, CommandLine):
		PSArgs.set_opacity(1.0)
		OptionsCheckBoxes.set_opacity(0.0)

	def on_Options_A_clicked(self, Options_A):
		if Options_A.get_active():
			self.option_A = True

		else:
			self.option_A = False

	def on_Options_F_clicked(self, Options_F):
		if Options_F.get_active():
			self.option_F = True

		else:
			self.option_F = False

	def on_Options_U_clicked(self, Options_U):
		if Options_U.get_active():
			self.option_U = True

		else:
			self.option_U = False

	def on_Options_L_clicked(self, Options_L):
		if Options_L.get_active():
			self.option_L = True
		
		else:
			self.option_L = False

	def on_Options_X_clicked(self, Options_X):
		if Options_X.get_active():
			self.option_X = True

		else:
			self.option_X = False

	def on_Options_E_clicked(self, Options_E):
		if Options_E.get_active():
			self.option_E = True

		else:
			self.option_E = False
			

builder = Gtk.Builder()
builder.add_from_file("Gui.glade")
builder.connect_signals(SignalHandler())

win = builder.get_object("MainWindow")
win.show_all()
Gtk.main()
