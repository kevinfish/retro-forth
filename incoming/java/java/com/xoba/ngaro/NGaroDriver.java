package com.xoba.ngaro;

import java.io.File;
import java.io.IOException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.TimeZone;

import com.xoba.ngaro.NGaroVM.ISaveImageController;
import com.xoba.ngaro.inf.IMemory;
import com.xoba.ngaro.inf.IOManager;
import com.xoba.ngaro.inf.IReplIOManager;

public class NGaroDriver {

	private static final ISaveImageController SIC = new DatestampedImageController(new File("."));

	public static void main(String[] args) throws Exception {
		if (false) {
			IMemory mem = createMemory();
			IReplIOManager rm = new ReplIOManager();
			IOManager im = new InputManager();
			NGaroVM vm = new NGaroVM(128, 1024, mem, im, rm, SIC);
			vm.run();
		} else {
			runTests();
		}
	}

	private static void runTests() throws Exception {
		for (String f : new String[] { "files.rx", "base.rx", "core.rx", "vocabs.rx" }) {
			IReplIOManager rm = new ReplIOManager();
			rm.includeFile("test/" + f);
			IOManager im = new InputManager();
			IMemory mem = createMemory();
			NGaroVM vm = new NGaroVM(128, 1024, mem, im, rm, SIC);
			vm.run();
			System.out.println("********************************************************* DONE");
		}
	}

	private static IMemory createMemory() throws IOException {
		return NGaroVM.initializeMemory(new File("retroImage"), true, 1000000);
	}

	public static class DatestampedImageController implements ISaveImageController {

		private final File dir;

		public DatestampedImageController(File dir) {
			this.dir = dir;
		}

		@Override
		public File getSavedImageFile() {
			DateFormat df = new SimpleDateFormat("yyyyMMdd'T'HHmmss'Z'");
			df.setTimeZone(TimeZone.getTimeZone("GMT"));
			return new File(dir, "retroImage_" + df.format(new Date()));
		}

	}
}
