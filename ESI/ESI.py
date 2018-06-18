import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk

#for df
import subprocess
from subprocess import Popen, PIPE

class App:
	def __init__(self):
		self.builder = Gtk.Builder()
		self.builder.add_from_file("Gui.glade")
		self.builder.connect_signals(SignalHandler())
		
class SignalHandler:
	def __init__(self):
		self.option_k = False
		self.option_P = False
		self.option_t = False
		self.option_h = False
		self.firstTime = True

	def onDestroy(self, *args):
		Gtk.main_quit()

	def on_Quit_activate(self, *args):
		Gtk.main_quit()
		
	def on_MenuSave_activate(self, *args):
		start = app.builder.get_object("OutputText").get_buffer().get_start_iter()
		end = app.builder.get_object("OutputText").get_buffer().get_end_iter()
		output = app.builder.get_object("OutputText").get_buffer().get_text(start,end,True)
		
		if not output:
			self.errMessage = Gtk.MessageDialog(type=Gtk.MessageType.ERROR, buttons=Gtk.ButtonsType.OK, text = "There is no output to save!")
			self.errResponse = self.errMessage.run()
			if self.errResponse == Gtk.ResponseType.OK:
				self.errMessage.destroy() 	
			
		else:
			self.fcd = Gtk.FileChooserDialog("Save...", None, Gtk.FileChooserAction.SAVE, (Gtk.STOCK_CANCEL, Gtk.ResponseType.CANCEL, Gtk.STOCK_SAVE, Gtk.ResponseType.OK))
			self.response = self.fcd.run()
			if self.response == Gtk.ResponseType.CANCEL:
				self.fcd.destroy()
			
			elif self.response == Gtk.ResponseType.OK:
				self.filename = self.fcd.get_filename()
				open(self.filename, 'w').write(output)
				self.fcd.destroy()
	
	def on_ActionBo_changed(self, ActionBox):
			
		if ActionBox.get_active() == 1:
			app.builder.get_object("OutputText").get_buffer().set_text("")
			app.builder.get_object("ActionBo").set_active(0)
			
		elif ActionBox.get_active() == 2:
			start = app.builder.get_object("OutputText").get_buffer().get_start_iter()
			end = app.builder.get_object("OutputText").get_buffer().get_end_iter()
			clipboard = Gtk.Clipboard.get(Gdk.SELECTION_CLIPBOARD)
			clipboard.set_text(app.builder.get_object("OutputText").get_buffer().get_text(start,end,True), len = -1)
			clipboard.store()	
			app.builder.get_object("ActionBo").set_active(0)
			
		else:
			pass

	def on_ExecuteButton_clicked(self, ExecuteButton):
		psArgs = app.builder.get_object("PSArgs").get_text()

		if not psArgs:

			if self.option_k:
				psArgs += "k"

			if self.option_P:
				psArgs += "P"

			if self.option_t:
				psArgs += "t"
		
			if self.option_h:
				psArgs += "h"					
		
		if self.firstTime == True:
			app.builder.get_object("ActionBox").set_opacity(1.0)
			app.builder.get_object("OutputText").set_opacity(1.0)
			self.firstTime = False
		
		
		if not psArgs:
			proc = subprocess.Popen(['df'], stdout = subprocess.PIPE)
			
		else:
			proc = subprocess.Popen(['df', ("-" + psArgs)], stdout = subprocess.PIPE)

		out, err = proc.communicate()
		
		if not out:
			print ("BLAD!")
				
		else:
			app.builder.get_object("OutputText").get_buffer().set_text(out.decode("utf-8"))			

	def on_Basic_clicked(self, Basic):
		app.builder.get_object("OptionsCheckBoxes").set_opacity(1.0)
		app.builder.get_object("PSArgs").set_opacity(0.0)
	
	def on_CommandLine_clicked(self, CommandLine):
		app.builder.get_object("PSArgs").set_opacity(1.0)
		app.builder.get_object("OptionsCheckBoxes").set_opacity(0.0)

	def on_Options_k_clicked(self, Options_k):
		if Options_k.get_active():
			self.option_k = True

		else:
			self.option_k = False

	def on_Options_P_clicked(self, Options_P):
		if Options_P.get_active():
			self.option_P = True

		else:
			self.option_P = False

	def on_Options_t_clicked(self, Options_t):
		if Options_t.get_active():
			self.option_t = True

		else:
			self.option_t = False

	def on_Options_h_clicked(self, Options_h):
		if Options_h.get_active():
			self.option_h = True
		
		else:
			self.option_h = False
			
if __name__ == "__main__":
	app = App()
	window = app.builder.get_object("MainWindow")
	window.show_all()
	Gtk.main()
