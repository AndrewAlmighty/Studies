import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk

#for ps
import subprocess
from subprocess import Popen, PIPE

class App:
	def __init__(self):
		self.builder = Gtk.Builder()
		self.builder.add_from_file("Gui.glade")
		self.builder.connect_signals(SignalHandler())
		
class SignalHandler:
	def __init__(self):
		self.option_A = False
		self.option_F = False
		self.option_U = False
		self.option_L = False
		self.option_X = False
		self.option_E = False
		self.firstTime = True

	def onDestroy(self, *args):
		Gtk.main_quit()

	def on_Quit_activate(self, *args):
		Gtk.main_quit()
		
	def on_MenuSave_activate(self, *args):
		pass
		
	def on_MenuOpen_activate(self, *args):
		pass
	
	def on_ActionBo_changed(self, ActionBox):
			
		if ActionBox.get_active() == 1:
			app.builder.get_object("OutputText").get_buffer().set_text("")
			
		elif ActionBox.get_active() == 2:
			start = app.builder.get_object("OutputText").get_buffer().get_start_iter()
			end = app.builder.get_object("OutputText").get_buffer().get_end_iter()
			clipboard = Gtk.Clipboard.get(Gdk.SELECTION_CLIPBOARD)
			clipboard.set_text(app.builder.get_object("OutputText").get_buffer().get_text(start,end,True), len = -1)
			clipboard.store()			
			
		else:
			pass

	def on_ExecuteButton_clicked(self, ExecuteButton):
		psArgs = app.builder.get_object("PSArgs").get_text()
		print(psArgs)
		if not psArgs:

			if self.option_A:
				psArgs += "a"

			if self.option_F:
				psArgs += "f"

			if self.option_U:
				psArgs += "u"
		
			if self.option_L:
				psArgs += "l"

			if self.option_X:
				psArgs += "x"

			if self.option_E:
				psArgs += "e"
					
		
		if self.firstTime == True:
			app.builder.get_object("ActionBox").set_opacity(1.0)
			app.builder.get_object("OutputText").set_opacity(1.0)
			self.firstTime = False
		
		
		if not psArgs:
			proc = subprocess.Popen(['ps'], stdout = subprocess.PIPE)
			
		else:
			proc = subprocess.Popen(['ps', psArgs], stdout = subprocess.PIPE)

		out, err = proc.communicate()
		app.builder.get_object("OutputText").get_buffer().set_text(out.decode("utf-8"))
		app.builder.get_object("OutputText").get_buffer().get_end_iter()
			

	def on_Basic_clicked(self, Basic):
		app.builder.get_object("OptionsCheckBoxes").set_opacity(1.0)
		app.builder.get_object("PSArgs").set_opacity(0.0)
	
	def on_CommandLine_clicked(self, CommandLine):
		app.builder.get_object("PSArgs").set_opacity(1.0)
		app.builder.get_object("OptionsCheckBoxes").set_opacity(0.0)

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
			
if __name__ == "__main__":
	app = App()
	window = app.builder.get_object("MainWindow")
	window.show_all()
	Gtk.main()
