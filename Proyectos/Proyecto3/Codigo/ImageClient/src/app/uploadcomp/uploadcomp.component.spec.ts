import { ComponentFixture, TestBed } from '@angular/core/testing';

import { UploadcompComponent } from './uploadcomp.component';

describe('UploadcompComponent', () => {
  let component: UploadcompComponent;
  let fixture: ComponentFixture<UploadcompComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ UploadcompComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(UploadcompComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
